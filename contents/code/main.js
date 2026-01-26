/*global effect, effects, set, cancel, Effect */

"use strict";

const blacklist = [
  // Spectacle needs to be blacklisted in order to stay out of its own screenshots.
  "spectacle spectacle", // x11
  "spectacle org.kde.spectacle" // wayland
];

class WayMcaEffect {
  constructor() {
    // KWin will load the core-profile variant from contents/shaders/waymca_core.frag
    // when we pass contents/shaders/waymca.frag here.
    this.shader = effect.addFragmentShader(Effect.MapTexture, "waymca.frag");

    effect.configChanged.connect(this.loadConfig.bind(this));
    effects.windowAdded.connect(this.onWindowAdded.bind(this));
    effects.windowClosed.connect(this.onWindowClosed.bind(this));

    this.loadConfig();

    for (const window of effects.stackingOrder) {
      this.applyToWindow(window);
    }
  }

  loadConfig() {
    const radius = effect.readConfig("Radius", 2.0);
    const strength = effect.readConfig("Strength", 1.0);

    effect.setUniform(this.shader, "uRadius", radius);
    effect.setUniform(this.shader, "uStrength", strength);
    effect.setUniform(this.shader, "uEnabled", 1.0);

    for (const window of effects.stackingOrder) {
      this.applyToWindow(window);
    }
  }

  static shouldApply(window) {
    if (!window) {
      return false;
    }

    if (blacklist.indexOf(window.windowClass) !== -1) {
      return false;
    }

    // Avoid the AltTab popup and similar overlays.
    if (!window.hasDecoration && window.onAllDesktops) {
      return false;
    }

    // Don't apply to combobox popups, tooltips, popup menus, etc.
    if (window.popupWindow) {
      return false;
    }

    if (window.lockScreen || window.outline) {
      return false;
    }

    if (!window.managed || !window.visible || window.deleted) {
      return false;
    }

    // We don't want most plasmashell windows, except real decorated dialogs.
    if (window.windowClass === "plasmashell plasmashell" ||
        window.windowClass === "plasmashell org.kde.plasmashell") {
      return window.hasDecoration;
    }

    return window.normalWindow || window.dialog;
  }

  applyToWindow(window) {
    if (effects.hasActiveFullScreenEffect) {
      return;
    }

    if (!WayMcaEffect.shouldApply(window)) {
      this.cleanup(window);
      return;
    }

    if (window.waymcaShaderBinding) {
      return;
    }

    // Keep a persistent binding so the fragment shader is used for this window.
    window.waymcaShaderBinding = set({
      window,
      duration: 1,
      animations: [{
        type: Effect.ShaderUniform,
        fragmentShader: this.shader,
        uniform: "uEnabled",
        from: 1.0,
        to: 1.0
      }]
    });
  }

  cleanup(window) {
    if (!window || !window.waymcaShaderBinding) {
      return;
    }

    cancel(window.waymcaShaderBinding);
    delete window.waymcaShaderBinding;
  }

  onWindowAdded(window) {
    this.applyToWindow(window);
  }

  onWindowClosed(window) {
    this.cleanup(window);
  }
}

new WayMcaEffect();
