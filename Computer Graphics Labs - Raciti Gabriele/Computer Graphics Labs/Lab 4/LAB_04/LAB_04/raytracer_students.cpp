#include "raytracer.h"
#include "material.h"
#include "vectors.h"
#include "argparser.h"
#include "raytree.h"
#include "utils.h"
#include "mesh.h"
#include "face.h"
#include "sphere.h"

// casts a single ray through the scene geometry and finds the closest hit
bool
RayTracer::CastRay (Ray & ray, Hit & h, bool use_sphere_patches) const
{
  bool answer = false;
  Hit nearest;
  nearest = Hit();

  // intersect each of the quads
  for (int i = 0; i < mesh->numQuadFaces (); i++)
  {
	Face *f = mesh->getFace (i);
	if (f->intersect (ray, h, args->intersect_backfacing))
	{
		if( h.getT() < nearest.getT() )
		{
			answer = true;
			nearest = h;
		}
	}
  }

  // intersect each of the spheres (either the patches, or the original spheres)
  if (use_sphere_patches)
  {
	for (int i = mesh->numQuadFaces (); i < mesh->numFaces (); i++)
	{
	  Face *f = mesh->getFace (i);
	  if (f->intersect (ray, h, args->intersect_backfacing))
	  {
		if( h.getT() < nearest.getT() )
		{
			answer = true;
			nearest = h;
		}
	  }
	}
  }
  else
  {
	const vector < Sphere * >&spheres = mesh->getSpheres ();
	for (unsigned int i = 0; i < spheres.size (); i++)
	{
	  if (spheres[i]->intersect (ray, h))
	  {
		if( h.getT() < nearest.getT() )
		{
			answer = true;
			nearest = h;
		}
	  }
	}
  }

  h = nearest;
  return answer;
}

Vec3f
RayTracer::TraceRay (Ray & ray, Hit & hit, int bounce_count) const
{

  hit = Hit ();
  bool intersect = CastRay (ray, hit, false);

  if( bounce_count == args->num_bounces )
  	RayTree::SetMainSegment (ray, 0, hit.getT () );
  else
	RayTree::AddReflectedSegment(ray, 0, hit.getT() );

  Vec3f answer = args->background_color;

  Material *m = hit.getMaterial ();
  if (intersect == true)
  {
	assert (m != NULL);
	Vec3f normal = hit.getNormal ();
	Vec3f point = ray.pointAtParameter (hit.getT ());

	// ----------------------------------------------
	// ambient light
	answer = args->ambient_light * m->getDiffuseColor ();

	// ----------------------------------------------
	// if the surface is shiny...
	Vec3f reflectiveColor = m->getReflectiveColor ();

	// ==========================================
	// ASSIGNMENT:  ADD REFLECTIVE LOGIC
	// ==========================================
	
	//se questo punto della superficie non � per nulla riflettente non va usato il ray-tracing
	if (reflectiveColor.Length() != 0 && bounce_count > 0) {
		//******* RICORSIONE DEL RAYTRACING ********
		Vec3f VRay = ray.getDirection();
		Vec3f reflectionRay = VRay - (2 * VRay.Dot3(normal) * normal);
		reflectionRay.Normalize();
		Ray* new_ray = new Ray(point, reflectionRay);

		answer += TraceRay(*new_ray, hit, bounce_count - 1) * reflectiveColor;
	}
	
	// ----------------------------------------------
	// add each light
	Hit* new_hit;
	bool colpito;
	Ray* n_ray;
	Vec3f n_point, dista, pointOnLight, dirToLight;


	int num_lights = mesh->getLights ().size ();
	for (int i = 0; i < num_lights; i++)
	{
	  // ==========================================
	  // ASSIGNMENT:  ADD SHADOW LOGIC
	  // ==========================================
	  Face *f = mesh->getLights ()[i];
	  Vec3f pointOnLight = f->computeCentroid ();
	  Vec3f dirToLight = pointOnLight - point;
	  dirToLight.Normalize ();
	  extern bool softShadow;
	  //SOFT SHADOWS	
	  if (args->softShadow) {
		  int hMAX = 600;
		  for (int h = 0; h < hMAX; h++) {
			  new_hit = new Hit();
			  pointOnLight = f->RandomPoint();			//scelta casuale di un punto nell'area della luce	
			  dirToLight = pointOnLight - point;
			  dirToLight.Normalize();
			  n_ray = new Ray(point, dirToLight);
		  }
	  }

	  // creo shadow ray verso il punto luce
	  n_ray = new Ray(point, dirToLight);
	  new_hit = new Hit();
	  colpito = CastRay(*n_ray, *new_hit, false);

	  // controllo se il primo oggetto � colpito da tale raggio
	  if (colpito) {
		  n_point = n_ray->pointAtParameter(new_hit->getT());
		  // calcola il vettore di stanza fra il punto colpito del raggio e il punto sulla luce
		  dista.Sub(dista, n_point, pointOnLight);

		  if (dista.Length() < 0.01) {	//il punto colpito coincide con la luce
			  
			  
			  if (normal.Dot3(dirToLight) > 0)		// se e' la sorgente luminosa i-esima allora
			  {
				  // calcolare e aggiungere ad answer il contributo luminoso
				  Vec3f lightColor = 0.2 * f->getMaterial()->getEmittedColor() * f->getArea();
				  answer += m->Shade(ray, hit, dirToLight, lightColor, args);
			  }
			  // altrimenti la luce i non contribuisce alla luminosita' di point.
		  }
	  }

      
	  
	  

	  
	  

	}
    
  }

  return answer;
}