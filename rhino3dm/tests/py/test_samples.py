import os
import subprocess
import unittest

class TestRhino3dmSamples(unittest.TestCase):

    def test_samples(self):

        samplesDir = os.path.dirname(os.getcwd())
        samplesDir = os.path.dirname(samplesDir)
        samplesDir = os.path.join(samplesDir, "py")

        os.chdir(samplesDir)

        for file in os.listdir(samplesDir):
            if file.endswith(".py"):
                filepath = os.path.join(samplesDir, file)
                
                cmd = subprocess.run(["python", filepath], capture_output=True)

                with self.subTest(msg=filepath):
                    #print(cmd.returncode)
                    if cmd.returncode != 0:
                        print(cmd.stderr.decode())
                    #else:
                        #stdout = cmd.stdout.decode()
                        #print(stdout)
                    #print(filepath)
                    self.assertEqual(cmd.returncode, 0)
                    
                    

if __name__ == '__main__':
    print("running tests")
    unittest.main()
    print("tests complete")
