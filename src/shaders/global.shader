#define MACRO(hash, name) hash name
MACRO(#,version 330)

#ifndef VS
#error "pipo"
#endif

#if VS
#define VAR out
#else
#define VAR in
#endif

#include "common.glsl"

VAR vec2 fragTexCoord;
VAR vec4 fragColor;

#if VS

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;

#if INSTANCING
in mat4 instanceTransform;
#endif

uniform mat4 mvp;

void main()
{
	fragTexCoord = vertexTexCoord;
	fragColor = vertexColor;
#if INSTANCING
	mat4 tr = instanceTransform;
	mat4 mvpi = mvp * tr;
	gl_Position = mvpi * vec4(vertexPosition, 1.0);
#else
	gl_Position = mvp * vec4(vertexPosition, 1.0);
#endif
}

#else //!VS

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main()
{
	vec4 texelColor = texture(texture0, fragTexCoord);
	finalColor = texelColor * colDiffuse * fragColor;
}

#endif
