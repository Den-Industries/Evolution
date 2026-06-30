uniform sampler2D texture;
uniform float blur_radius;
uniform vec4 color;

void main()
{
	float shtuka = 0; 
	for (int x = -5; x <= 5; x++)
		for (int y = -5; y <= 5; y++)
			shtuka = shtuka + texture2D(texture, gl_TexCoord[0].xy + vec2(x * blur_radius, y * blur_radius)).a;
	gl_FragColor = vec4(
	shtuka * 0.01 * texture2D(texture, gl_TexCoord[0].xy).r + (1.0 - shtuka * 0.01) * color.x, 
	shtuka * 0.01 * texture2D(texture, gl_TexCoord[0].xy).g + (1.0 - shtuka * 0.01) * color.y, 
	shtuka * 0.01 * texture2D(texture, gl_TexCoord[0].xy).b + (1.0 - shtuka * 0.01) * color.z, 
	shtuka * 0.01 * color.w);
	//texture2D(texture, gl_TexCoord[0].xy) + 
}
