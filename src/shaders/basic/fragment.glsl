#version 330 core

out vec4 fragColor;

uniform int uTextureSample;
uniform vec3 uKd;
uniform sampler2D uSampler;

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTextureCoord;

void main(void) {
  if (uTextureSample == 1) {
    fragColor = texture(uSampler, vTextureCoord);
  } else {
    fragColor = vec4(uKd, 1.0);
  }
}
