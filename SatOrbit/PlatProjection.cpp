#include "PlatProjection.h"

Point2D GeoToGrid(double width,double height,Point2D geo)
{
	Point2D m_grid;
	m_grid.x=width/2.0+(geo.x/360.0)*width;
	m_grid.y=height/2.0+(geo.y/180.0)*height;
	return m_grid;
}