#pragma	once

// Exist for UI Modal effect (modal)
namespace NSGUI
{
	// block only the mouse (short form modal)
    BOOL IsCharMoveBlock();
	void SetCharMoveBlock();
	void ResetCharMoveBlock();

	// Block both keyboard and mouse (modal)
	BOOL IsCharMoveAllBlock();
	void SetCharMoveAllBlock();
	void ResetCharMoveAllBlock();
}