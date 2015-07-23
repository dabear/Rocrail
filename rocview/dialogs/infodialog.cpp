/*
 Rocrail - Model Railroad Software

 Copyright (c) 2002 Robert Jan Versluis, Rocrail.net

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
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "infodialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include "rocs/public/str.h"
#include "rocs/public/trace.h"
#include "rocview/wrapper/public/Gui.h"
#include "rocview/res/icons.hpp"
#include "rocrail/wrapper/public/Plan.h"

#include "infodialog.h"

////@begin XPM images
////@end XPM images
//#include "rocview/xpm/rocrail-logo-net.xpm"
#include "rocview/public/guiapp.h"

/*!
 * InfoDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( InfoDialog, wxDialog )

/*!
 * InfoDialog event table definition
 */

BEGIN_EVENT_TABLE( InfoDialog, wxDialog )

////@begin InfoDialog event table entries
////@end InfoDialog event table entries

END_EVENT_TABLE()

/*!
 * InfoDialog constructors
 */

InfoDialog::InfoDialog( )
{
}

InfoDialog::InfoDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Create(parent, id, caption, pos, size, style);


  m_Splash->SetBitmap( *_img_rocrail_logo );
  m_Splash->Refresh();

  char* str = StrOp.fmt("%s revision %d", wGui.buildDate, wxGetApp().getRevisionNr());
  m_Build->SetLabel( wxString(str,wxConvUTF8) );
  StrOp.free( str );

  m_Home->SetLabel( _T("http://www.rocrail.net") );
  m_Support->SetLabel( _T("TM and Copyright 2002-2015 Robert Jan Versluis.\nP.O.Box 1122\nD-67369 Dudenhofen") );
  m_labLic->SetLabel( _T("All Rights Reserved.") );
  //m_ThanksLine->SetLabel( wxGetApp().getMsg( "license" ) );
  m_ThanksLine->SetLabel( _T("") );

  m_RocrailVersion->SetLabel( wxT("Rocrail ") + wxString( wPlan.getrocrailversion(wxGetApp().getModel()),wxConvUTF8) );
  m_RocrailPwd->SetLabel( wxString( wPlan.getrocrailpwd(wxGetApp().getModel()),wxConvUTF8) );

  m_Thanks->AppendText( _T("The name Rocrail and the associated logo is our trademark and is officially registered in Germany under number 302008050592 and in the Benelux under number 928454.") );
  m_Thanks->AppendText( _T("\n\n") );

  m_Thanks->SetInsertionPoint(0);
  m_Thanks->ShowPosition(0);

  GetSizer()->Layout();
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
  Centre();

  m_Splash->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( InfoDialog::OnSplash ), NULL, this );

}

InfoDialog::~InfoDialog( )
{
  m_Splash->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( InfoDialog::OnSplash ), NULL, this );
}

/*!
 * InfoDialog creator
 */

bool InfoDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin InfoDialog member initialisation
    m_Splash = NULL;
    m_Build = NULL;
    m_RocrailVersion = NULL;
    m_RocrailPwd = NULL;
    m_Home = NULL;
    m_Support = NULL;
    m_labLic = NULL;
    m_ThanksLine = NULL;
    m_Thanks = NULL;
////@end InfoDialog member initialisation

////@begin InfoDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end InfoDialog creation
    return true;
}

/*!
 * Control creation for InfoDialog
 */

void InfoDialog::CreateControls()
{
////@begin InfoDialog content construction
    InfoDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(0, 1, 0, 0);
    itemBoxSizer2->Add(itemFlexGridSizer3, 1, wxGROW|wxALL, 5);

    m_Splash = new wxStaticBitmap( itemDialog1, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(m_Splash, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_Build = new wxStaticText( itemDialog1, wxID_STATIC_INFO_BUILD, _("build"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    m_Build->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Ubuntu")));
    itemFlexGridSizer3->Add(m_Build, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Server"));
    wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
    itemFlexGridSizer3->Add(itemStaticBoxSizer6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

    m_RocrailVersion = new wxStaticText( itemDialog1, wxID_ANY, _("Rocrail Version"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer6->Add(m_RocrailVersion, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);

    m_RocrailPwd = new wxStaticText( itemDialog1, wxID_ANY, _("."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer6->Add(m_RocrailPwd, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

    m_Home = new wxStaticText( itemDialog1, wxID_STATIC_INFO_HOME, _("home"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemFlexGridSizer3->Add(m_Home, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP|wxADJUST_MINSIZE, 5);

    m_Support = new wxStaticText( itemDialog1, wxID_STATIC_INFO_SUPPORT, _("support"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemFlexGridSizer3->Add(m_Support, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP|wxADJUST_MINSIZE, 5);

    m_labLic = new wxStaticText( itemDialog1, wxID_ANY, _("License"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemFlexGridSizer3->Add(m_labLic, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_ThanksLine = new wxStaticText( itemDialog1, WX_STATICTEXT_INFO_THANKS, _("Special Thanks to:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer3->Add(m_ThanksLine, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP|wxADJUST_MINSIZE, 5);

    m_Thanks = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_INFO_THANKS, wxEmptyString, wxDefaultPosition, wxSize(-1, 100), wxTE_MULTILINE|wxTE_READONLY );
    itemFlexGridSizer3->Add(m_Thanks, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    itemFlexGridSizer3->AddGrowableRow(0);
    itemFlexGridSizer3->AddGrowableCol(0);

    // Connect events and objects
    m_Build->Connect(wxID_STATIC_INFO_BUILD, wxEVT_LEFT_UP, wxMouseEventHandler(InfoDialog::onBuildRevision), NULL, this);
////@end InfoDialog content construction
}

/*!
 * Should we show tooltips?
 */

bool InfoDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap InfoDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin InfoDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end InfoDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon InfoDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin InfoDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end InfoDialog icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BITMAPBUTTON_INFO_SPLASH
 */

void InfoDialog::OnSplash( wxMouseEvent& event )
{
  EndModal( wxID_OK );
}



/*!
 * wxEVT_LEFT_UP event handler for wxID_STATIC_INFO_BUILD
 */

void InfoDialog::onBuildRevision( wxMouseEvent& event )
{
  char* url = StrOp.fmt("https://github.com/rocrail/Rocrail/commit/%s", wxGetApp().getCommitHash() );
  wxLaunchDefaultBrowser(wxString(url,wxConvUTF8), wxBROWSER_NEW_WINDOW );
  StrOp.free(url);
}




