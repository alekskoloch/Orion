uniform sampler2D texture;
uniform float u_time;
uniform float u_intensity;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    if (u_intensity <= 0.0)
    {
        gl_FragColor = texture2D(texture, uv) * gl_Color;
        return;
    }

    float noise = sin(uv.y * 50.0 + u_time * 20.0) * 0.005 * u_intensity;
    float jitter = fract(sin(dot(vec2(u_time, uv.y), vec2(12.9898, 78.233))) * 43758.5453) * 0.01 * u_intensity;

    vec4 rValue = texture2D(texture, uv + vec2(noise + jitter, 0.0));
    vec4 gValue = texture2D(texture, uv);
    vec4 bValue = texture2D(texture, uv - vec2(noise + jitter, 0.0));

    vec4 finalColor = vec4(rValue.r, gValue.g, bValue.b, gValue.a);

    gl_FragColor = finalColor * gl_Color;
}