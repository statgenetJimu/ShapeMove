/*
 * Implicit Fairing of Arbitrary Meshes using Diffusion and Curvature Flow
 * Mathieu Desbrun, Mark Meyer, Peter Schröder, Alan H. Barr
 * ACM Siggraph '99 Proceedings
 */

#ifndef DDG_APPLICATION_H
#define DDG_APPLICATION_H


namespace DDG
{
   class Application
   {
   public:

      void process(const double step, spiny::Mesh& meshY)
      {
         meshY.setCurvatureChange4(step);
         meshY.updateDeformation();
         meshY.updateDeformation2();
         
      }

   protected:
   };
}

#endif
