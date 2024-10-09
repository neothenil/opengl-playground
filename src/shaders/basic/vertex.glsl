#version 330 core

layout (location = 0) in vec3 aVertexPosition;
layout (location = 1) in vec3 aNormalPosition;
layout (location = 2) in vec2 aTextureCoord;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTextureCoord;

void main(void) {
  vFragPos = aVertexPosition;
  vNormal = aNormalPosition;
  vTextureCoord = aTextureCoord;
  gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aVertexPosition, 1.0);
  // gl_Position = vec4(aVertexPosition.y, aVertexPosition.x, aVertexPosition.z, 1.0);
}
