#pragma once

[[nodiscard]] inline ShaderProgramRef CreateBaseSceneShader()
{
	const char* vertexShaderText = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;

uniform mat4 pvMatrix;

out vec2 fragmentTexCoord;
out vec4 fragmentColor;

void main()
{
	gl_Position      = pvMatrix * vec4(aPosition, 1.0);
	fragmentTexCoord = aTexCoord;
	fragmentColor    = aColor;
}
)";

	const char* fragmentShaderText = R"(
in vec2 fragmentTexCoord;
in vec4 fragmentColor;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
	FragColor = texture(Texture, fragmentTexCoord) * fragmentColor;
}
)";

	return GetRenderSystem().CreateShaderProgram({vertexShaderText}, {fragmentShaderText});
}

[[nodiscard]] inline ShaderProgramRef CreateMeshSceneShader()
{
	const char* vertexShaderText = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexCoord;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 fragmentNormal;
out vec3 fragmentColor;
out vec2 fragmentTexCoord;


void main()
{
	gl_Position = uProjection * uView * uWorld * vec4(aPosition, 1.0);
	fragmentNormal = aNormal;
	fragmentColor = aColor;
	fragmentTexCoord = aTexCoord;
}
)";

	const char* fragmentShaderText = R"(
in vec3 fragmentNormal;
in vec3 fragmentColor;
in vec2 fragmentTexCoord;

out vec4 FragColor;

uniform sampler2D DiffuseTexture;

void main()
{
	vec4 textureClr = texture(DiffuseTexture, fragmentTexCoord) * vec4(fragmentColor, 1.0);
	if (textureClr.a < 0.02) discard;
	FragColor = textureClr;
}
)";

	return GetRenderSystem().CreateShaderProgram({ vertexShaderText }, { fragmentShaderText });
}