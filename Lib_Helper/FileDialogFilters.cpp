#include "stdafx.h"
#include "./FileDialogFilters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RFILE_DIALOG_FILTERS
{
	CString strFilterAnimationCFG = "CFG (*.cfg)|*.cfg*|";
	CString strFilterAnimationBIN = "BIN (*.bin)|*.bin*|";

	CString strFilterPiece = "Piece (*.cps,*.aps,*.vps)|*.cps*;*.aps*;*.vps*;|CPS (*.cps)|*.cps*|APS (*.aps)|*.aps*|VPS (*.vps)|*.vps*|";
	CString strFilterPieceCPS = "Piece CPS (*.cps)|*.cps*|";
	CString strFilterPieceAPS = "Piece APS (*.aps)|*.aps*|";
	CString strFilterPieceVPS = "Piece VPS (*.vps)|*.vps*|";

	CString strFilterContainer = "Container (*.chf,*.abf,*.vcf)|*.chf*;*.abf*;*.vcf*;|CHF (*.chf)|*.chf*|ABF (*.abf)|*.abf*|VCF (*.vcf)|*.vcf*|";
	CString strFilterContainerCHF = "Container CHF (*.chf)|*.chf*|";
	CString strFilterContainerABF = "Container ABF (*.abf)|*.abf*|";
	CString strFilterContainerVCF = "Container VCF (*.vcf)|*.vcf*|";

	CString strFilterBoneLinkABL = "ABL (*.abl)|*.abl*|";

	CString strFilterEffSkin = "effSkin (*.effskin)|*.effskin*|";

	CString strFilterEGP = "EGP (*.egp)|*.egp*|";

	CString strFilterSkin = "Skin (*.x,*.mxf)|*.x*;*.mxf*;|X (*.x)|*.x*;|MXF (*.mxf)|*.mxf*;|";

	CString strFilterTexture = "Textures (*.dds,*.bmp,*.tga,*.jpg,*.mtf)|*.dds*;*.bmp*;*.tga*;*.jpg*;*.mtf*;|DDS (*.dds*)|*.dds*|BMP (*.bmp*)|*.bmp*|TGA (*.tga*)|*.tga*|JPG (*.jpg*)|*.jpg*|MTF (*.mtf*)|*.mtf*|";


};