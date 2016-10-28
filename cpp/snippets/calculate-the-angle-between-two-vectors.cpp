// Description: Calculates the angle between two 3-D vectors.
// Parameters:
//  v0           - [in]  The first angle.
//  v1           - [in]  The second angle.
//  reflex_angle - [out] The reflex angle.
// Returns: The angle in radians.
double ON_3dVectorAngle( ON_3dVector v0, ON_3dVector v1, double* reflex_angle = 0 )
{
  // Unitize the input vectors
  v0.Unitize();
  v1.Unitize();

  double dot = ON_DotProduct( v0, v1 );

  // Force the dot product of the two input vectors to
  // fall within the domain for inverse cosine, which
  // is -1 <= x <= 1. This will prevent runtime
  // "domain error" math exceptions.
  dot = ( dot < -1.0 ? -1.0 : ( dot > 1.0 ? 1.0 : dot ) );

  double angle = acos( dot );

  if( reflex_angle )
    *reflex_angle = (ON_PI * 2) - angle;

  return angle;
}
