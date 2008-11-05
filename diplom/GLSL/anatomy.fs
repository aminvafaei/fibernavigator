uniform sampler3D texes[10];
uniform bool show[10];
uniform float threshold[10];
uniform float alpha[10];
uniform int type[10];
uniform int countTextures;

vec3 defaultColorMap( float value )
{
    value *= 5.0;
	vec3 color;

	if( value < 0.0 )
		color = vec3( 0.0, 0.0, 0.0 );
    else if( value < 1.0 )
		color = vec3( 0.0, value, 1.0 );
	else if( value < 2.0 )
		color = vec3( 0.0, 1.0, 2.0-value );
    else if( value < 3.0 )
		color =  vec3( value-2.0, 1.0, 0.0 );
    else if( value < 4.0 )
		color = vec3( 1.0, 4.0-value, 0.0 );
    else if( value <= 5.0 )
		color = vec3( 1.0, 0.0, value-4.0 );
    else
		color =  vec3( 1.0, 0.0, 1.0 );
	return color;
}

void lookupTex(inout vec4 col, in int type, in sampler3D tex, in float threshold, in float alpha)
{
	vec3 col1 = vec3(0.0);
	if (type == 3)
	{
		col1.r = clamp( texture3D(tex, gl_TexCoord[0].xyz).r, 0.0, 1.0);

		if (col1.r - threshold > 0.0)
		{
			col.rgb = ((1.0 - alpha) * col.rgb) + (alpha * defaultColorMap( col1.r));
		}
	}

	else
	{
		col1.r = clamp( texture3D(tex, gl_TexCoord[0].xyz).r, 0.0, 1.0);
		col1.g = clamp( texture3D(tex, gl_TexCoord[0].xyz).g, 0.0, 1.0);
		col1.b = clamp( texture3D(tex, gl_TexCoord[0].xyz).b, 0.0, 1.0);

		if ( ((col1.r + col1.g + col1.b) / 3.0 - threshold) > 0.0)
		{
			col.rgb =  ((1.0 - alpha) * col.rgb) + (alpha * col1.rgb);
		}
		col.a += clamp (( (col.r*3.0) + (col.g*3.0) + (col.b*3.0) ), 0.0, 1.0) - threshold;
	}
}


void main()
{
	vec4 col = vec4(0.0, 0.0, 0.0, 0.0);

	for (int i = 9 ; i > -1 ; i--)
	{
		if (show[i]) lookupTex(col, type[i], texes[i], threshold[i], alpha[i]);
	}

	col = clamp(col, 0.0, 1.0);
	gl_FragColor = col;
}
