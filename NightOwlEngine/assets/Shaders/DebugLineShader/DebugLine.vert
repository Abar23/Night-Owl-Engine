layout (location = 0) in vec3 linePosition;

uniform mat4 viewProjectionMatrix;

void main()
{
    gl_Position = viewProjectionMatrix * vec4(linePosition, 1.0);
}