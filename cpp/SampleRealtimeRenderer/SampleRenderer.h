#pragma once

// This class creates a render thread to process rendering in the background

class CSampleRenderer
{
public:
	CSampleRenderer(RhRdk::Realtime::ISignalUpdate* pSignalUpdateInterface = nullptr);
	virtual ~CSampleRenderer();

	// To control rendering process
	bool StartRenderProcess(const ON_2iSize& frameSize);
	void StopRenderProcess();

	// Tells if the renderer is or should be running
	bool Running() const;

	IRhRdkRenderWindow* RenderWindow() const { return m_pRenderWnd; }

private:
	// Static method that is executed as a thread.
	// pData will be a pointer to an instance of this class.
	static unsigned int RenderProcess(void* pData);

	// Used by Running()
	bool m_bRunning;

	// Pointer to the thread object
	CWinThread* m_pRenderThread;

	// Dib to contain the rendered image - thread safe
	IRhRdkRenderWindow* m_pRenderWnd = nullptr;

	// Signal the display pipeline to redraw.
	RhRdk::Realtime::ISignalUpdate* m_pSignalUpdateInterface = nullptr;
};