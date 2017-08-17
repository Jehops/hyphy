/*
 
 HyPhy - Hypothesis Testing Using Phylogenies.
 
 Copyright (C) 1997-now
 Core Developers:
 Sergei L Kosakovsky Pond (sergeilkp@icloud.com)
 Art FY Poon    (apoon@cfenet.ubc.ca)
 Steven Weaver (sweaver@temple.edu)
 
 Module Developers:
 Lance Hepler (nlhepler@gmail.com)
 Martin Smith (martin.audacis@gmail.com)
 
 Significant contributions from:
 Spencer V Muse (muse@stat.ncsu.edu)
 Simon DW Frost (sdf22@cam.ac.uk)
 
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "HYEventTypes.h"
#include "HYTextBox.h"
#include "HYGraphicPane.h"

#ifdef    __HYPHYDMALLOC__
#include "dmalloc.h"
#endif
//__________________________________________________________________

_HYTextBox::_HYTextBox (_HYRect r,Ptr p, bool bt):_HYComponent (r,p),_HYPlatformTextBox()
{
    backColor.R       = backColor.G
                        = backColor.B
                          = 255;

    foreColor.R       = foreColor.G
                        = foreColor.B
                          = 0;


    editBoxFont.size  = 10;
    editBoxFont.style = HY_FONT_PLAIN;
    editBoxFont.face  = "Helvetica";

    alignFlags        = HY_ALIGN_LEFT;

    margins.left      = margins.right
                        = margins.top
                          = margins.bottom
                            = 5;

    boxFlags = HY_TB_ENABLED | (bt?HY_TB_BIGBOX:0);

    //boxType             = bt;
}

//__________________________________________________________________

_HYTextBox::~_HYTextBox()
{
}

//__________________________________________________________________

void            _HYTextBox::SetBackColor (_HYColor c)
{
    if ((c.R!=backColor.R)||(c.G!=backColor.G)||(c.B!=backColor.B)) {
        backColor = c;
        _SetBackColor (c);
        _MarkForUpdate();
    }
}

//__________________________________________________________________

_HYColor&       _HYTextBox::GetBackColor (void)
{
    return backColor;
}

//__________________________________________________________________

void            _HYTextBox::SetForeColor (_HYColor c)
{
    if ((c.R!=foreColor.R)||(c.G!=foreColor.G)||(c.B!=foreColor.B)) {
        foreColor = c;
        _SetForeColor (c);
        _MarkForUpdate();
    }
}

//__________________________________________________________________

void            _HYTextBox::SetBackTColor (_HYColor c)
{
    if ((c.R!=backTextColor.R)||(c.G!=backTextColor.G)||(c.B!=backTextColor.B)) {
        backTextColor = c;
        _SetBackTColor (c);
        _MarkForUpdate();
    }
}

//__________________________________________________________________

_HYColor&       _HYTextBox::GetForeColor (void)
{
    return foreColor;
}

//__________________________________________________________________

_HYColor&       _HYTextBox::GetBackTColor (void)
{
    return backTextColor;
}


//__________________________________________________________________
void            _HYTextBox::SetText (const _String& newText, bool update)
{
    _SetText (newText);
    _MarkForUpdate();
    if (messageRecipient && update) {
        messageRecipient->ProcessEvent (generateTextEditChangeEvent (GetID(),1));
    }
}

//__________________________________________________________________
void            _HYTextBox::InsertText (const _String& newText, bool update, bool append)
{
    _InsertText (newText, append);
    _MarkForUpdate();
    if (messageRecipient && update) {
        messageRecipient->ProcessEvent (generateTextEditChangeEvent (GetID(),1));
    }
}

//__________________________________________________________________

_String         _HYTextBox::GetText (void)
{
    return _GetText();
}


//__________________________________________________________________

void            _HYTextBox::StoreText (_String*& rec, bool selOnly)
{
    return _StoreText(rec, selOnly);
}

//__________________________________________________________________

_HYFont&        _HYTextBox::GetFont (void)
{
    return editBoxFont;
}

//__________________________________________________________________
void            _HYTextBox::SetMargins (_HYRect m)
{
    if ((m.top!=margins.top)||(m.bottom!=margins.bottom)
            ||(m.left!=margins.left)||(m.right!=margins.right)) {
        margins = m;
        SetVisibleSize (rel);
        _MarkForUpdate ();
    }
}

//__________________________________________________________________

void            _HYTextBox::SetFont (_HYFont&f)
{
    if ((!f.face.Equal(&editBoxFont.face))||(f.size!=editBoxFont.size)||(f.style!=editBoxFont.style)) {
        _SetFont (f);
        editBoxFont.face  = f.face;
        editBoxFont.size  = f.size;
        editBoxFont.style = f.style;
        _MarkForUpdate();
    }
}

//__________________________________________________________________
void            _HYTextBox::SetVisibleSize (_HYRect rel)
{
    _HYComponent::SetVisibleSize (rel);
    _HYPlatformTextBox::_SetVisibleSize (rel);
}

//__________________________________________________________________

void            _HYTextBox::EnableTextEdit (bool e)
{
    bool ie = boxFlags&HY_TB_ENABLED;
    if (ie!=e) {
        if (e) {
            boxFlags |= HY_TB_ENABLED;
        } else {
            boxFlags -= HY_TB_ENABLED;
        }

        _EnableTextBox (e);
        _MarkForUpdate ();
    }
}

//__________________________________________________________________

void            _HYTextBox::FocusComponent (void)
{
#ifndef __WINDOZE__
    if (!(boxFlags&HY_TB_FOCUSED))
#endif
    {
        boxFlags |= HY_TB_FOCUSED;
        if (!(boxFlags&HY_TB_BIGBOX)) {
            SetSelection   (0,0x7fffffff);
        }
        _FocusComponent();
        _MarkForUpdate();
    }
}

//__________________________________________________________________

void            _HYTextBox::UnfocusComponent (void)
{
    if (boxFlags&HY_TB_FOCUSED) {
        boxFlags -= HY_TB_FOCUSED;
        _UnfocusComponent();
        _MarkForUpdate();
    }
}
