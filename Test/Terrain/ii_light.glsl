#line 1

// Material
struct ii_Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shininess;
};

uniform ii_Material ii_FrontMaterial;
uniform ii_Material ii_BackMaterial;

// Lighting

const int ii_MaxLights = 8;

struct ii_LightModelParameters
{
    vec4 ambient;   // Acs
};

uniform ii_LightModelParameters  ii_LightModel;

struct ii_LightSourceParameters
{
    vec4    ambient;                // Acli
    vec4    diffuse;                // Dcli
    vec4    specular;               // Scli
    vec4    position;               // Ppli
    vec4    halfVector;             // derived: Hi
    vec3    spotDirection;          // Sdli
    float   spotExponent;           // Srli
    float   spotCutoff;             // Crli - (range: [0.0,90.0], 180.0)
    float   spotCosCutoff;          // derived: cos(crli) - range: [1.0,0.0],-1.0
    float   constantAttenuation;    // K0
    float   linearAttenuation;      // K1
    float   quadraticAttenuation;   // K2
};

uniform ii_LightSourceParameters    ii_LightSource[ii_MaxLights];

uniform bool ii_lightingMode;

varying vec4 ii_FrontColor;
//varying vec4 ii_BackColor;

void DirectionalLight( in int i,
                       in vec3 normal,
                       inout vec4 ambient,
                       inout vec4 diffuse,
                       inout vec4 specular )
{
    float nDotVP = max(0.0, dot(normal, vec3(ii_LightSource[i].position)));

    //float nDotHV = max(0.0, dot(normal, vec3(ii_LightSource[i].halfVector)));

    float nDotHV = max(0.0, dot(normal, vec3(0,0,1)));
    float pf;

    if( nDotVP == 0.0 )
        pf = 0.0;
    else 
        pf = pow(nDotHV, ii_FrontMaterial.shininess);

    ambient  += ii_LightSource[i].ambient;
    diffuse  += ii_LightSource[i].diffuse * nDotVP;
    specular += ii_LightSource[i].specular * pf;
}

void ii_lighting( in vec3 normal )
{
    /*
    if( ii_lightingMode == false )
    {
        ii_FrontColor = vec4(0,1,0,1);
        return;
    }
    */

    // assume one light for now.... 

    vec4 amb = vec4(0.0);
    vec4 diff = vec4(0.0);
    vec4 spec = vec4(0.0);

    DirectionalLight( 0, normal, amb, diff, spec );

    ii_FrontColor =  (amb  * ii_FrontMaterial.ambient) +
                     (diff * ii_FrontMaterial.diffuse) +
                     (spec * ii_FrontMaterial.specular)  ;

//ii_FrontColor = vec4( normal, 1. );
    //ii_BackColor = ii_FrontColor;
}

