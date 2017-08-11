#version 130

varying vec2 varTexCoord;
varying float varTime;
varying float varTTL;
varying float varIsGradient;

uniform vec4 uColor;
uniform sampler2D uTex;
uniform vec4 uStartColor;
uniform vec4 uFinalColor;

void main()
{
	vec4 tex = texture(uTex, varTexCoord);
	vec4 Gradient;

	if (varIsGradient != 0.0)
		Gradient = mix(uStartColor, uFinalColor, varTime / varTTL);

	if (varIsGradient == 0.0)
	{
		if (tex.xyz != vec3(0))
			gl_FragColor = uColor * tex;
		else
			gl_FragColor = uColor;
	}

	
	if (varIsGradient != 0.0)
	{
		gl_FragColor = Gradient * tex;
	}
	if (gl_FragColor.w < 0.1)
		discard;
}