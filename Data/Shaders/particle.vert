attribute vec3 aPos;
attribute vec2 aUV;
attribute vec3 aPoses;
attribute vec3 aTimes;
attribute vec4 aColors;

varying vec3 varPos;
varying vec2 varTexCoord;
varying vec4 varColor;

uniform mat4 uView;
uniform mat4 uProjection;

uniform vec4 uPosition;
uniform vec2 uSize;
uniform vec2 uStartSize;
uniform vec2 uFinalSize;
uniform float uScaleOL;
uniform float uBillboard;

uniform int uRenderMode;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}


void main(void)
{
	mat4 Rotation = rotationMatrix(vec3(0, 0, 1), aTimes.z / 90);
	mat4 ModelView = uView * mat4(1);

	ModelView[0][0] = 1;
	ModelView[0][1] = 0;
	ModelView[0][2] = 0;

	ModelView[1][0] = 0;
	ModelView[1][1] = 1;
	ModelView[1][2] = 0;

	ModelView[2][0] = 0;
	ModelView[2][1] = 0;
	ModelView[2][2] = 1;

	vec3 pos = aPoses;

	float lifePercent = aTimes.x / aTimes.y;

	vec3 Pos = aPos;
	if (uBillboard != 0.0)
	{
		if (uScaleOL != 0.0)
		{
			vec2 SizeOverLifetime = mix(uStartSize, uFinalSize, lifePercent);
			Position = uProjection * (uView * vec4(pos, 1.0) + vec4(aPos, 0.0) * vec4(SizeOverLifetime, 1.0, 0.0) * Rotation);
		} else
		{
			Position = Rotation * uProjection * (uView * vec4(pos, 1.0) + vec4(aPos * vec3(uSize, 1.0), 0.0));
		}
	}
	else
	{
		if (uScaleOL != 0.0)
		{
			vec2 SizeOverLifetime = mix(uStartSize, uFinalSize, lifePercent);
			Position = uProjection * uView * (vec4(pos, 1.0) + vec4(aPos * vec3(SizeOverLifetime, 1.0), 0.0) * Rotation);
		} else
		{
			Position = uProjection * uView * (vec4(pos, 1.0) + vec4(aPos * vec3(uSize, 1.0), 0.0) * Rotation);
		}
	}

	varPos = pos + aPos;
	varTexCoord = aUV;
	varColor = aColors;
}



