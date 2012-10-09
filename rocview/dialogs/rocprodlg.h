/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2012 Rob Versluis, Rocrail.net

 Without an official permission commercial use is not permitted.
 Forking this project is not permitted.

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
#ifndef __rocprodlg__
#define __rocprodlg__

/**
@file
Subclass of RocProDlgGen, which is generated by wxFormBuilder.
*/

#include "rocprodlggen.h"

#include "rocs/public/node.h"
#include "rocs/public/map.h"

//// end generated include

/** Implementing RocProDlgGen */
class RocProDlg : public RocProDlgGen
{
  char* m_DecFilename;
  iONode m_DecNode;
  iOMap m_CVMap;
  bool parseDecFile();
	protected:
		// Handlers for RocProDlgGen events.
		void onTreeSelChanged( wxTreeEvent& event );
		void onOpen( wxCommandEvent& event );
		void onOK( wxCommandEvent& event );
    void onClose( wxCloseEvent& event );
	public:
		/** Constructor */
		RocProDlg( wxWindow* parent );
    void event(iONode node);
	
};

#endif // __rocprodlg__
