# We will try to read the root file using the newly installed python package called 'uproot'
import uproot
import numpy as np

f1 = uproot.open("ROOT_IN/ts_simulation.root")["LDMX_Events"]

a1 = f1.array("trigScintDigisTag_sim.pe_")
a2 = f1.array("trigScintDigisTag_sim")
a3 = f1.array("trigScintDigisTag_sim.barID_")

b1 = np.array(a1)
b3 = np.array(a3)

np.savetxt("TXT_IN/nd.txt",a2,fmt="%d")

f2 = open("TXT_IN/pe.txt", "w")
for row in b1:
    np.savetxt(f2, row,delimiter=',',fmt="%d")
f2.close()

f3 = open("TXT_IN/barID.txt", "w")
for row in b3:
    np.savetxt(f3, row,delimiter=',',fmt="%d")
f3.close()
