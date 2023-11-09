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

[[nodiscard]] inline ShaderProgramRef CreateTileSceneShader()
{
	const char* vertexShaderText = R"(
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormal;
layout(location = 2) in vec2 inputTexCoord;
layout(location = 3) in vec4 inputColor;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragmentNormal;
out vec4 fragmentColor;
out vec2 fragmentTexCoord;

void main()
{
	gl_Position      = projectionMatrix * viewMatrix * worldMatrix * vec4(inputPosition, 1.0);
	fragmentNormal   = normalize(mat3(worldMatrix) * inputNormal);
	fragmentColor    = inputColor;
	fragmentTexCoord = inputTexCoord;
}
)";

	const char* fragmentShaderText = R"(
in vec3 fragmentNormal;
in vec4 fragmentColor;
in vec2 fragmentTexCoord;

out vec4 FragColor;

uniform sampler2D DiffuseTexture;

void main()
{
	vec4 textureColor = texture(DiffuseTexture, fragmentTexCoord);
	vec4 diffuseColor = textureColor * fragmentColor;
	if (diffuseColor.a < 0.02) discard;

	vec3 lightDirection = vec3(-0.2, -0.5, 1.0);
	vec3 diffuseLightColor = vec3(1.0, 1.0, 1.0);
	float minimalIntensity = 0.7;

	vec3 lightDir = -lightDirection;
	
	// Calculate the amount of light on this pixel.
	float lightIntensity = clamp(dot(fragmentNormal, lightDir), minimalIntensity, 1.0);
	vec3 lightColor = clamp((diffuseLightColor * lightIntensity), minimalIntensity, 1.0);

	FragColor = vec4(lightColor, 1.0) * diffuseColor;
}
)";

	return GetRenderSystem().CreateShaderProgram({ vertexShaderText }, { fragmentShaderText });
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

[[nodiscard]] inline ShaderProgramRef CreateNewMeshSceneShader()
{
	const char* vertexShaderText = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aTangents;
layout(location = 3) in vec4 aColor;
layout(location = 4) in vec2 aTexCoord;
layout(location = 5) in vec2 aTexCoord2;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 fragmentNormal;
out vec4 fragmentTangents;
out vec4 fragmentColor;
out vec2 fragmentTexCoord;
out vec2 fragmentTexCoord2;

void main()
{
	gl_Position = uProjection * uView * uWorld * vec4(aPosition, 1.0);
	fragmentNormal = aNormal;
	fragmentTangents = aTangents;
	fragmentColor = aColor;
	fragmentTexCoord = aTexCoord;
	fragmentTexCoord2 = aTexCoord2;
}
)";

	const char* fragmentShaderText = R"(
in vec3 fragmentNormal;
in vec4 fragmentTangents;
in vec4 fragmentColor;
in vec2 fragmentTexCoord;
in vec2 fragmentTexCoord2;

out vec4 FinalFragmentColor;

uniform sampler2D DiffuseTexture;

void main()
{
	vec4 textureClr = texture(DiffuseTexture, fragmentTexCoord) * fragmentColor;
	if (textureClr.a < 0.02) discard;

	FinalFragmentColor = textureClr;
}
)";

	return GetRenderSystem().CreateShaderProgram({ vertexShaderText }, { fragmentShaderText });
}

[[nodiscard]] inline ShaderProgramRef CreateTestShader()
{
	const char* vertexShaderText = R"(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 projectionMatrix;

out vec2 TexCoord;

void main()
{
	gl_Position = projectionMatrix * vec4(aPosition, 1.0);
	TexCoord    = aTexCoord;
}
)";

	const char* fragmentShaderText = R"(
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
	FragColor = texture(Texture, TexCoord);
}
)";

	return GetRenderSystem().CreateShaderProgram({ vertexShaderText }, { fragmentShaderText });
}