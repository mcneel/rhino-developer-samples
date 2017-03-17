#pragma once

// This class creates a render thread to process rendering in the background

class CSampleRenderer
{
public:
  CSampleRenderer();
  virtual ~CSampleRenderer();

  // To control rendering process
  void StartRenderProcess();
  void StopRenderProcess();

  // Tells if the renderer is or should be running
  bool Running() const;

private:
  // Static method that is executed as a thread.
  // pData will be a pointer to an instance of this class.
  static unsigned int RenderProcess(void* pData);

  // Used by Running()
  bool m_bRunning;

  // Pointer to the thread object
  CWinThread* m_pRenderThread;
};