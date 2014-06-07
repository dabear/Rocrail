/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2014 Rob Versluis, Rocrail.net




 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#include "wx/wxprec.h"

#ifndef WX_PRECOMP
  #include "wx/wx.h"
#endif

#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/math.h>



#include "rocs/public/node.h"
#include "rocs/public/str.h"
#include "rocs/public/trace.h"
#include "rocs/public/file.h"

#include "rocview/public/meter.h"

static int modulal(int val)
{
  do{
    val +=360;
  }while(val<0);
  return val;
}

static double getRadians( double degrees ) {
  static double PI= 3.14159265358979;
  return (degrees / 360) * (2.0 * PI);
}



Meter::Meter(wxWindow *parent, wxWindowID id, int x, int y)
                  : wxPanel(parent, id,  wxPoint(x, y), wxSize(110,110), wxBORDER_NONE)
{
  m_Parent = parent;
  m_iSpeed = 0;
  m_iMaxSpeed = 240;
  speed = 216;
  speed = ((modulal((90 - speed)) * M_PI) / 180);
  SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));

  Connect( wxEVT_PAINT, wxPaintEventHandler( Meter::OnPaint ) );
  //Disconnect( wxEVT_PAINT, wxPaintEventHandler( MyPanel1::OnPaint ) );
  TraceOp.trc( "meter", TRCLEVEL_INFO, __LINE__, 9999, "meter instantiated" );
}


void Meter::OnPaint(wxPaintEvent& event) {
  int width, height;

  //if( !IsShownOnScreen() )
    //return;

#if defined __WIN32
  SetBackgroundStyle(wxBG_STYLE_CUSTOM);
  wxBufferedPaintDC dc(this);
  wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
#else
  wxPaintDC dc(this);
  wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
#endif

#ifdef wxANTIALIAS_DEFAULT
  gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
#endif

  GetSize(&width, &height);

#if defined __WIN32
  //Background workaround
  gc->SetBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));
  gc->DrawRectangle(0, 0, width, height);
#endif

  TraceOp.trc( "meter", TRCLEVEL_INFO, __LINE__, 9999, "width=%d height=%d", width, height );

  if( height < width )
    width = height;

  double c = width/2;

  wxPen borderPen( wxColour(0, 0, 0), wxSOLID );
  borderPen.SetWidth(1);
  gc->SetPen( borderPen );
  gc->SetBrush(*wxWHITE_BRUSH);

  gc->DrawEllipse(0, 0, width-1, width-1);


  wxGraphicsPath platePath = gc->CreatePath();
  platePath.AddArc(width/2, width/2, width/2-8, getRadians(126), getRadians(56), true);
  wxPen grayPen( wxColour(100, 100, 100), wxSHORT_DASH );
  grayPen.SetWidth(1);
  gc->SetPen( grayPen );
  gc->StrokePath(platePath);


  int i;
  platePath = gc->CreatePath();
  wxPen platePen( wxColour(0, 0, 0), wxSOLID );
  for (i = 0; i < 25; i++) {
    double k = ((modulal((90 - (i * 6))) * M_PI) / 180);

    if( i%6 == 0 ) {
      platePath.MoveToPoint(c + 0.75 * c * cos(k), c - 0.75 * c * sin(k));
      platePath.AddLineToPoint(c + 0.95 * c * cos(k), c - 0.95 * c * sin(k));
    }
  }
  for (i = 36; i < 60; i++) {
    double k = ((modulal((90 - (i * 6))) * M_PI) / 180);

    if( i%6 == 0 ) {
      platePath.MoveToPoint(c + 0.75 * c * cos(k), c - 0.75 * c * sin(k));
      platePath.AddLineToPoint(c + 0.95 * c * cos(k), c - 0.95 * c * sin(k));
    }
  }
  platePen.SetWidth(2);
  gc->SetPen( platePen );
  gc->StrokePath(platePath);

  platePath = gc->CreatePath();
  for (i = 0; i < 25; i++) {
    double k = ((modulal((90 - (i * 6))) * M_PI) / 180);

    if( i%3 == 0 ) {
      platePath.MoveToPoint(c + 0.75 * c * cos(k), c - 0.75 * c * sin(k));
      platePath.AddLineToPoint(c + 0.85 * c * cos(k), c - 0.85 * c * sin(k));
    }
  }
  for (i = 36; i < 60; i++) {
    double k = ((modulal((90 - (i * 6))) * M_PI) / 180);

    if( i%3 == 0 ) {
      platePath.MoveToPoint(c + 0.75 * c * cos(k), c - 0.75 * c * sin(k));
      platePath.AddLineToPoint(c + 0.85 * c * cos(k), c - 0.85 * c * sin(k));
    }
  }
  platePen.SetWidth(1);
  gc->SetPen( platePen );
  gc->StrokePath(platePath);



  double b = width / 100.0;
  int fontsize = 7;
#if defined __APPLE__
  fontsize = 9;
#endif

  wxFont font(fontsize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  gc->SetFont(font,*wxBLACK);
  if( m_iMaxSpeed > 120 ) {
    gc->DrawText( wxT("0"), b * 30.0, b * 70.0 );
    gc->DrawText( wxT("30"), b * 18.0, b * 55.0 );
    gc->DrawText( wxT("60"), b * 18.0, b * 35.0 );
    gc->DrawText( wxT("90"), b * 27.0, b * 20.0 );
    gc->DrawText( wxT("120"), b * 42.0, b * 14.0 );
    gc->DrawText( wxT("150"), b * 60.0, b * 20.0 );
    gc->DrawText( wxT("180"), b * 68.0, b * 35.0 );
    gc->DrawText( wxT("210"), b * 68.0, b * 55.0 );
    gc->DrawText( wxT("240"), b * 60.0, b * 70.0 );
  }
  else {
    gc->DrawText( wxT("0"), b * 30.0, b * 70.0 );
    gc->DrawText( wxT("15"), b * 18.0, b * 55.0 );
    gc->DrawText( wxT("30"), b * 18.0, b * 35.0 );
    gc->DrawText( wxT("45"), b * 27.0, b * 20.0 );
    gc->DrawText( wxT("60"), b * 45.0, b * 14.0 );
    gc->DrawText( wxT("75"), b * 64.0, b * 20.0 );
    gc->DrawText( wxT("90"), b * 72.0, b * 35.0 );
    gc->DrawText( wxT("105"), b * 68.0, b * 55.0 );
    gc->DrawText( wxT("120"), b * 60.0, b * 70.0 );
  }

  drawNeedle(gc, c);

  delete gc;

}


void Meter::drawNeedle(wxGraphicsContext* gc, double c, bool erase) {
  gc->SetBrush( erase?*wxWHITE_BRUSH:wxColour(255, 0, 0) );
  wxPen redPen( erase?wxColour(255, 255, 255):wxColour(255, 0, 0), wxSOLID );
  redPen.SetWidth(2);
  gc->SetPen( redPen );

  wxGraphicsPath path = gc->CreatePath();
  path.MoveToPoint(c, c);
  path.AddLineToPoint(c + 0.80 * c * cos(speed), c - 0.80 * c * sin(speed));
  gc->StrokePath(path);

  wxPen blackPen( wxColour(0, 0, 0), wxSOLID );
  blackPen.SetWidth(1);
  gc->SetPen( blackPen );
  gc->SetBrush(*wxGREY_BRUSH);
  gc->DrawEllipse(c-4, c-4, 8, 8);

}



void Meter::setSpeed(int iSpeed, int maxspeed) {
  /*
  speed = 90; // 175
  speed = 143.0; // 240 (120)
  speed = 216; // 0 (0)
  speed = 270; // 45
  speed = 0; // 120 (60)
  speed = 360; // 120 (60)
  */
  if( maxspeed > 0 )
    m_iMaxSpeed = maxspeed;

  m_iSpeed = iSpeed;

  if( m_iMaxSpeed > 120 ) {
    if( iSpeed <= 120 ) {
      double step = (360.0-216.0) / 120.0;
      speed = 216.0 + step*iSpeed;
    }
    else {
      if( iSpeed > 240 )
        iSpeed = 240;
      double step = 143.0 / 120.0;
      speed = step*(iSpeed-120);
    }
  }
  else {
    if( iSpeed <= 60 ) {
      double step = (360.0-216.0) / 60.0;
      speed = 216.0 + step*iSpeed;
    }
    else {
      if( iSpeed > 120 )
        iSpeed = 120;
      double step = 143.0 / 60.0;
      speed = step*(iSpeed-60);
    }
  }

  speed = ((modulal((90 - speed)) * M_PI) / 180);
  Refresh(true);
}

