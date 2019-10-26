import matplotlib.pyplot as plt
import numpy as np

def get_data(file_path):
    with open(file_path, 'r') as filename:
        data = filename.readlines()
        data_length = len(data)
        
        for line in range(data_length):
            data[line] = float(data[line].strip('\n'))
        
        x = np.arange(data_length)

        return [x, data]


if __name__ == "__main__":
    P = "./cmake-build-debug/P.txt"
    # file1 = "./data/telephone_number1.txt"
    outP1 = "./cmake-build-debug/outP.txt"
    Pm1 = "./cmake-build-debug/Pm.txt"


    plt.figure(0)
    plt.subplot(311); plt.bar(get_data(P)[0], get_data(P)[1]); plt.xlabel("P")
    plt.subplot(312); plt.bar(get_data(Pm1)[0], get_data(Pm1)[1]); plt.xlabel("Pm")
    plt.subplot(313); plt.bar(get_data(outP1)[0], get_data(outP1)[1]); plt.xlabel("outP")

    plt.figure(1)
    plt.bar(get_data(P)[0], get_data(P)[1]); plt.xlabel("P")
    
    plt.show()


