//
// cOrbit.cpp
//
// Copyright (c) 2002-2003 Michael F. Henry
//
// mfh 11/15/2003
// 
#include "stdafx.h"

#include "cOrbit.h"
#include "math.h"
#include "time.h"
#include "cVector.h"
#include "cEci.h"
#include "coord.h"
#include "cJulian.h"
#include "cNoradSGP4.h"
#include "cNoradSDP4.h"

//////////////////////////////////////////////////////////////////////
cOrbit::cOrbit(const cTle &tle) :
   m_tle(tle),
   m_pNoradModel(NULL)
{
   m_tle.Initialize();

   int    epochYear = (int)m_tle.getField(cTle::FLD_EPOCHYEAR);
   double epochDay  =      m_tle.getField(cTle::FLD_EPOCHDAY );

   if (epochYear < 57)
      epochYear += 2000;
   else
      epochYear += 1900;

   m_jdEpoch = cJulian(epochYear, epochDay);

   m_secPeriod = -1.0;

   // Recover the original mean motion and semimajor axis from the
   // input elements.
   double mm     = mnMotion();
   double rpmin  = mm * 2 * PI / MIN_PER_DAY;   // rads per minute

   double a1     = pow(XKE / rpmin, TWOTHRD);
   double e      = Eccentricity();
   double i      = Inclination();
   double temp   = (1.5 * CK2 * (3.0 * sqr(cos(i)) - 1.0) / 
                   pow(1.0 - e * e, 1.5));   
   double delta1 = temp / (a1 * a1);
   double a0     = a1 * 
                   (1.0 - delta1 * 
                   ((1.0 / 3.0) + delta1 * 
                   (1.0 + 134.0 / 81.0 * delta1)));

   double delta0 = temp / (a0 * a0);

   m_mnMotionRec        = rpmin / (1.0 + delta0);
   m_aeAxisSemiMinorRec = a0 / (1.0 - delta0);
   m_aeAxisSemiMajorRec = m_aeAxisSemiMinorRec / sqrt(1.0 - (e * e));
   m_kmPerigeeRec       = XKMPER_WGS72 * (m_aeAxisSemiMajorRec * (1.0 - e) - AE);
   m_kmApogeeRec        = XKMPER_WGS72 * (m_aeAxisSemiMajorRec * (1.0 + e) - AE);

   if (2.0 * PI / m_mnMotionRec >= 225.0)
   {
      // SDP4 - period >= 225 minutes.
      m_pNoradModel = new cNoradSDP4(*this);
   }
   else
   {
      // SGP4 - period < 225 minutes
      m_pNoradModel = new cNoradSGP4(*this);
   }
}

/////////////////////////////////////////////////////////////////////////////
cOrbit::~cOrbit()
{
   delete m_pNoradModel;
}

//////////////////////////////////////////////////////////////////////////////
// Return the period in seconds
double cOrbit::Period() const
{
   if (m_secPeriod < 0.0)
   {
      // Calculate the period using the recovered mean motion.
      if (m_mnMotionRec == 0)
         m_secPeriod = 0.0;
      else
         m_secPeriod = (2 * PI) / m_mnMotionRec * 60.0;
   }

   return m_secPeriod;
}

//////////////////////////////////////////////////////////////////////////////
// Returns elapsed number of seconds from epoch to given time.
// Note: "Predicted" TLEs can have epochs in the future.
double cOrbit::TPlusEpoch(const cJulian &gmt) const
{
   return gmt.spanSec(Epoch());
}

//////////////////////////////////////////////////////////////////////////////
// Returns the mean anomaly in radians at given GMT.
// At epoch, the mean anomaly is given by the elements data.
double cOrbit::mnAnomaly(cJulian gmt) const
{
   double span = TPlusEpoch(gmt);
   double P    = Period();

   assert(P != 0.0);

   return fmod(mnAnomaly() + (TWOPI * (span / P)), TWOPI);
}

//////////////////////////////////////////////////////////////////////////////
// getPosition()
// This procedure returns the ECI position and velocity for the satellite
// at "tsince" minutes from the (GMT) TLE epoch. The vectors returned in
// the ECI object are kilometer-based.
// tsince  - Time in minutes since the TLE epoch (GMT).
bool cOrbit::getPosition(double tsince, cEci *pEci) const
{
   bool rc;

   rc = m_pNoradModel->getPosition(tsince, *pEci);

   pEci->ae2km();

   return rc;
}
   
//////////////////////////////////////////////////////////////////////////////
// SatName()
// Return the name of the satellite. If requested, the NORAD number is
// appended to the end of the name, i.e., "ISS (ZARYA) #25544".
// The name of the satellite with the NORAD number appended is important
// because many satellites, especially debris, have the same name and
// would otherwise appear to be the same satellite in ouput data.
string cOrbit::SatName(bool fAppendId /* = false */) const
{
   string str = m_tle.getName();

   if (fAppendId)
   {
      string strId;

      m_tle.getField(cTle::FLD_NORADNUM, cTle::U_NATIVE, &strId);
      str = str + " #" + strId;
   }

   return str;
}

