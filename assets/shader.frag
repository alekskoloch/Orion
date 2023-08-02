uniform sampler2D texture;

void main()
{
    vec2 texCoords = gl_TexCoord[0].xy;
    
    vec4 sum = vec4(0.0);
    float kernel[5];
    kernel[0] = 0.05; kernel[1] = 0.25; kernel[2] = 0.4; kernel[3] = 0.25; kernel[4] = 0.05;
    float blurStrength = 1.0 / 400.0;

    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            sum += texture2D(texture, texCoords + vec2(i, j) * blurStrength) * kernel[i+2] * kernel[j+2];
        }
    }
    
    float average = (sum.r + sum.g + sum.b) / 3.0;
    gl_FragColor = vec4(average, average, average, sum.a);
}