#version 110

uniform vec2 globalResolution;
uniform float globalTime;

mat3 calcLookAtMatrix(vec3 origin, vec3 target, float roll) {
    vec3 rr = vec3(sin(roll), cos(roll), 0.0);
    vec3 ww = normalize(target - origin);
    vec3 uu = normalize(cross(ww, rr));
    vec3 vv = normalize(cross(uu, ww));

    return mat3(uu, vv, ww);
}

vec3 getRay(mat3 cameraMatrix, vec2 screenPosition, float lensLength) {
    return normalize(cameraMatrix * vec3(screenPosition, lensLength));
}

vec3 getRay(vec3 origin, vec3 target, vec2 screenPosition, float lensLength) {
    mat3 cameraMatrix = calcLookAtMatrix(origin, target, 0.0);

    return getRay(cameraMatrix, screenPosition, lensLength);
}


vec2 squareFrame(vec2 screenSize) {
    vec2 position = 2.0 * (gl_FragCoord.xy / screenSize.xy) - 1.0;
    position.x *= screenSize.x / screenSize.y;

    return position;
}

vec2 squareFrame(vec2 screenSize, vec2 coord) {
    vec2 position = 2.0 * (coord.xy / screenSize.xy) - 1.0;
    position.x *= screenSize.x / screenSize.y;

    return position;
}

float plane(vec3 position)
{
    return position.y;
}

float sphere(vec3 position, float radius)
{
    return length(position) - radius;
}

float box(vec3 position, vec3 dimension)
{
    position = abs(position) - dimension;
    return max(max(position.x, position.y), position.z);
}

float subtract(float a, float b)
{
    return max(-b, a);
}

vec2 union(vec2 a, vec2 b)
{
    return (a.x < b.x) ? a : b;
}

vec2 intersect(vec2 a, vec2 b)
{
    return (a.x > b.x) ? a : b;
}

vec2 scene(in vec3 position)
{
    float planeId   = 0.0;
    float sphereId  = 1.0;
    float boxId     = 2.0;
    float sphere2Id = 3.0;
    float sphere3Id = 4.0;

    float sphereRadius   = 0.6;
    float sphere3Radius  = 1.0;
    vec3  boxDimension   = vec3(0.8);

    vec3 sphereOffset  = vec3(-2.0, 1.0, -2.0);
    vec3 boxOffset     = vec3(-3.0, 1.0,  0.0);
    vec3 box2Offset    = vec3(-2.0, 1.0, -2.0);

    vec2 res = union(
        vec2(plane(position), planeId),
        vec2(sphere(position - sphereOffset, sphereRadius), sphereId)
    );

    vec2 intersection = intersect(
        vec2(box(position - boxOffset, vec3(0.8)), boxId),
        vec2(sphere(position - boxOffset, 1.1), sphere2Id)
    );

    vec2 subtraction = vec2(subtract(
        box(position - box2Offset, vec3(0.8)),
        sphere(position - box2Offset, sphere3Radius)
    ), sphere3Id);

    res = union(
        res,
        intersection
    );

    res = union(
        res,
        subtraction
    );

    return res;
}

vec2 testScene(in vec3 position)
{
    float sphereId      = 0.0;
    float sphere2Id     = 1.0;
    float sphereRadius  = 1.0;
    vec3  sphereOffset  = vec3(-2.0, 1.0, -2.0);
    vec3  sphere2Offset = vec3(2.5, 1.0,  -2.0);

    vec2 res = union(
        vec2(
            sphere(position - sphereOffset, sphereRadius),
            sphereId
        ),
        vec2(
            sphere(position - sphere2Offset, sphereRadius),
            sphere2Id
        )
    );

    return res;
}

vec3 calcLighting(vec3 normal, vec3 rayDirection) {

    vec3 lightDirection     = normalize(vec3(0.0, 4.0, 5.0));

    vec3 ambientColor       = vec3(0.05, 0.15, 0.2);
    float kAmbient          = clamp(0.5 + 0.5 * normal.y, 0.0, 1.0);
    vec3 ambient            = kAmbient * ambientColor;

    vec3 diffuseColor       = vec3(0.2, 0.6, 0.8);
    float kDiffuse          = clamp(dot(lightDirection, normal), 0.0, 1.0);
    vec3 diffuse            = kDiffuse * diffuseColor;

    vec3 specularColor      = vec3(1.0);
    float kSpecularExponent = 24.0;
    vec3 h                  = normalize(-rayDirection + lightDirection);
    float nFacing           = clamp(dot(lightDirection, normal), 0.0, 1.0);
    float kSpecular         = pow(clamp(dot(h, normal), 0.0, 1.0), kSpecularExponent);
    vec3 specular           = nFacing * kSpecular * specularColor;

    vec3 color              = ambient + diffuse + specular;

    return color;
}

vec3 calcNormal(in vec3 position, in float eps) {

    vec3 epsilon = vec3(eps, 0.0, 0.0);
    vec3 normal  = vec3(
        scene(position + epsilon.xyy).x - scene(position - epsilon.xyy).x,
        scene(position + epsilon.yxy).x - scene(position - epsilon.yxy).x,
        scene(position + epsilon.yyx).x - scene(position - epsilon.yyx).x
    );

    return normalize(normal);
}

vec2 castRay(in vec3 rayOrigin, in vec3 rayDirection, in float
        maxDistance, in float precision, in int steps)
{
    float latest    = precision * 2.0;
    float distance  = 0.0;
    float type      = -1.0;
    vec2  res       = vec2(-1.0, -1.0);

    for(int i = 0; i < steps; i++) {
        if (abs(latest) < precision || distance > maxDistance) {
            continue;
        }

        vec2 result = scene(rayOrigin + rayDirection * distance);

        latest     = result.x;
        type       = result.y;
        distance  += latest;
    }

    if (distance < maxDistance) {
        res = vec2(distance, type);
    }

    return res;
}

vec3 render(in vec3 rayOrigin, in vec3 rayDirection)
{
    vec3 color = vec3(0.015);
    vec2 res   = castRay(rayOrigin, rayDirection, 100.0, 0.00001, 100);

    if (res.x > -0.5) {
        vec3 position = rayOrigin + res.x * rayDirection;
        vec3 normal   = calcNormal(position, 0.1);
        color         = calcLighting(normal, rayDirection);
    }

    vec3 gamma = vec3(1.0 / 2.2);
    color      = pow(color, gamma);
    color      = clamp(color, 0.0, 1.0);

    return color;
}

void main()
{
    vec2 resolution      = globalResolution;
    float time           = globalTime * 0.1;
    float cameraAngle    = 4.0;
    float cameraHeight   = 5.0;
    float cameraPane     = 7.0;
    float cameraDistance = 3.5;
    vec3 rayOrigin      = vec3(cameraPane * sin(cameraAngle), cameraHeight, cameraDistance * cos(cameraAngle * time));
    vec3 rayTarget      = vec3(0.0, 0.0, 0.0);
    vec2 screenPosition = squareFrame(resolution);
    vec3 rayDirection   = getRay(rayOrigin, rayTarget, screenPosition, 2.0);

    vec3 color          = render(rayOrigin, rayDirection);

    gl_FragColor.rgb = color;
    gl_FragColor.a   = 1.0;
}
