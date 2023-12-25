#version 330 core

in vec3 vertexPosition, vertexNormal;
uniform vec3 theta;
uniform mat4 Translate_Matrix;
uniform mat4 Inverse_Translate_Matrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec2 vertexTexture;

out vec3 normal, position;
out vec2 texCoord;

void main()
{
    vec3 angles = radians(theta);
    vec3 c = cos(angles);
    vec3 s = sin(angles);

    mat4 rx = mat4( 1.0,  0.0,  0.0, 0.0,
		    0.0,  c.x,  s.x, 0.0,
		    0.0, -s.x,  c.x, 0.0,
		    0.0,  0.0,  0.0, 1.0 );

    mat4 ry = mat4( c.y, 0.0, -s.y, 0.0,
		    0.0, 1.0,  0.0, 0.0,
		    s.y, 0.0,  c.y, 0.0,
		    0.0, 0.0,  0.0, 1.0 ); 

    mat4 rz = mat4( c.z, -s.z, 0.0, 0.0,
		    s.z,  c.z, 0.0, 0.0,
		    0.0,  0.0, 1.0, 0.0,
		    0.0,  0.0, 0.0, 1.0 );

    normal = mat3(transpose(inverse(model))) * vertexNormal;
    position = vec3(model * vec4(vertexPosition, 1.0));

    // Combine all transformations
    texCoord = vertexTexture;

    gl_Position = projection * view * model * Translate_Matrix * ry * Inverse_Translate_Matrix * vec4(vertexPosition, 1.0);
}