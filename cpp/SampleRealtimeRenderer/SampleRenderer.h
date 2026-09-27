#pragma once

#include <atomic>
#include <thread>

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

	// Used by Running().  The main thread clears it in StopRenderProcess()
	// and the render thread reads it, so it has to be atomic.
	std::atomic<bool> m_bRunning{false};

	// The render thread itself
	std::thread m_RenderThread;

	// Dib to contain the rendered image - thread safe
	IRhRdkRenderWindow* m_pRenderWnd = nullptr;

	// Signal the display pipeline to redraw.
	RhRdk::Realtime::ISignalUpdate* m_pSignalUpdateInterface = nullptr;
};