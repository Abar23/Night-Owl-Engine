#version 460 core

out vec4 FragColor;
in vec4 outCol;

void main(void){
FragColor = outCol;
} 