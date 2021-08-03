import numpy as np
import matplotlib.pyplot as plt
import math


# def checkOneSide(lux, stableLux, last):
#     if(stableLux == -1 or lux == stableLux or (last - stableLux)/(lux - stableLux) > 0):
#         return True
#     else:
#         return False


# def calStable(lux_list, lastStable):

#     std = np.std(lux_list)
#     RMS = math.sqrt(np.sum(np.array(lux_list)**2) / len(lux_list)) 
#     delta = lastStable - RMS
#     print("std:", std, "\tdelta:", delta, "\tmean:", np.mean(lux_list))
#     if lastStable == -1:
#         return RMS
#     else:
#         return lastStable - (delta / (std + 1))
#         # return np.mean(lux_list)


# def AIBrightness(lux_list, maxLux):
#     temp = []
#     stable_list = []
#     stable_idx = []
#     stableLux = -1
#     noiseNum = 0
#     noiseNum_th = 1
#     checkSize = 4
#     for idx, lux in enumerate(lux_list):

#         if(len(temp) == 0):
#             temp.append(lux)
#         else:
#             bOnOneSide = checkOneSide(lux, stableLux, temp[-1])
#             if bOnOneSide:
#                 temp.append(lux)
#             else:
#                 noiseNum += 1

#             if(noiseNum >= noiseNum_th):
#                 noiseNum = 0
#                 temp.clear()
#                 temp.append(lux)
        
#             checkSize = 4 if lux > temp[0] else 6

#             if len(temp) >= checkSize:
#                 lastStable = stableLux
#                 stableLux = calStable(temp, lastStable)
#                 stableLux = stableBySection(stableLux, lastStable, maxLux)
#                 stable_list.append(stableLux)
#                 stable_idx.append(idx)
#                 noiseNum = 0
#                 temp.clear()

#     return stable_list, stable_idx


# def draw(lux_list, stable_list, stable_idx):

#     X = np.linspace(0, len(lux_list)*0.2, len(lux_list))
#     # X = np.arange(0, len(lux_list)*0.2, 0.2)
#     plt.plot(X, np.array(lux_list), marker='.')  # , mec='r', mfc='r', markersize=6
#     stable_idx = [X[int(idx)] for idx in stable_idx]
#     plt.plot(np.array(stable_idx), np.array(stable_list), marker='.')  # , mec='g', mfc='g', markersize=6
#     # plt.xticks(X)

#     plt.show()


# def stableBySection(curLux, lastStable, maxLux):
#     if(lastStable == -1):
#         return curLux
#     ### large
#     # left
#     STABLE_DOWN_MIN_PERCENT = 0.15
#     STABLE_DOWN_SELF_PERCENT = 0.33
#     big_left = max(lastStable - maxLux*STABLE_DOWN_MIN_PERCENT, lastStable*STABLE_DOWN_SELF_PERCENT)
#     # right
#     STABLE_UP_MIN_PERCENT = 0.17
#     STABLE_UP_SELF_PERCENT = 1.7
#     big_right = min(lastStable + maxLux*STABLE_UP_MIN_PERCENT, lastStable*STABLE_UP_SELF_PERCENT)

#     ### small
#     # left
#     STABLE_SMALL_CHANGE_PERCENT = 0.3
#     STABLE_SMALL_CHANGE_TARGET_PERCENT = 0.1
#     small_left = lastStable - (lastStable - big_left) * STABLE_SMALL_CHANGE_PERCENT
#     # right
#     small_right = lastStable + (big_right - lastStable) * STABLE_SMALL_CHANGE_PERCENT


#     newLux = curLux

#     if curLux < big_left:
#         newLux =  big_left
#     elif curLux < small_left:
#         newLux =  curLux - (curLux - big_left) * STABLE_SMALL_CHANGE_TARGET_PERCENT
#     elif curLux > big_right:
#         newLux =  big_right
#     elif curLux > small_right:
#         newLux =  curLux + (big_right - curLux) * STABLE_SMALL_CHANGE_TARGET_PERCENT
#     else:
#         print("stable")
    
#     print("stableBySection:  ", big_left,",", small_left,",", lastStable,",", small_right,",", big_right, "/cur:", curLux, "/new:", newLux)
#     return newLux


def readFromRecord(path):
    with open(path, 'r') as f:
        data = f.read()
        if len(data) == 0: 
            return []
        data = data.split(',')
        data = [int(string) for string in data]
        return data


def checkOneSide(lux, stableLux, last):
    if stableLux == -1 or lux == stableLux or (last - stableLux)/(lux - stableLux) > 0:
        return True
    else:
        return False


def calRMSStable(lux_list):
    RMS = math.sqrt(np.sum(np.array(lux_list)**2) / len(lux_list)) 
    return RMS


def calSTDRMSStable(lux_list, lastStable):
    std = np.std(lux_list)
    RMS = math.sqrt(np.sum(np.array(lux_list)**2) / len(lux_list)) 
    delta = lastStable - RMS
    return lastStable - (delta / (std + 1))


def AIBrightness(lux_list, need_small, always_usd_std):
    temp = []
    stable_list = []
    stable_idx = []
    noiseNum = 0
    noiseNum_th = 1
    checkSize = 4
    isDownFromLowLux = False
    DARK_LUX_THRESHOLD = 36
    for idx, lux in enumerate(lux_list):
        if len(temp) == 0:
            temp.append(lux)
        else:
            if len(stable_list) > 0:
                isDownFromLowLux = (stable_list[-1] < DARK_LUX_THRESHOLD) and (lux <= stable_list[-1])
                bOnOneSide = checkOneSide(lux, stable_list[-1], temp[0])
            else:
                bOnOneSide = checkOneSide(lux, -1, temp[0])
            if bOnOneSide:
                temp.append(lux)
            else:
                noiseNum += 1

            if(noiseNum >= noiseNum_th):
                noiseNum = 0
                temp = []
                temp.append(lux)
                continue
        
            if len(stable_list) == 0 or lux > stable_list[-1]:
                checkSize = 4
            else:
                if isDownFromLowLux:
                    checkSize = 10
                else:
                    checkSize = 6

            if len(temp) >= checkSize:
                stableLux = -1
                if always_usd_std:
                    if len(stable_list) > 0:
                        stableLux = calSTDRMSStable(temp, stable_list[-1])
                        stableLux = stableBySection(stableLux, stable_list[-1], need_small)
                    else:
                        stableLux = calRMSStable(temp)
                else:
                    stableLux = calRMSStable(temp)
                    if isDownFromLowLux and stableLux < stable_list[-1]:
                        stableLux = calSTDRMSStable(temp, stable_list[-1])
                    if len(stable_list) > 0:
                        stableLux = stableBySection(stableLux, stable_list[-1], need_small)

                stable_list.append(stableLux)
                stable_idx.append(idx)
                noiseNum = 0
                temp = []

    return stable_list, stable_idx

def stableBySection(curLux, lastStable, need_small):
    if(lastStable == -1):
        return curLux
    ### large
    # left
    big_left = lastStable
    if lastStable < 30:
        big_left = lastStable / 4.0
    elif lastStable >= 30 and lastStable < 500:
        big_left = lastStable / 10.0 + 4.5
    elif lastStable >= 500 and lastStable < 8600:
        big_left = lastStable / 1.5 - 278.83
    
    # right
    big_right = lastStable
    if lastStable < 20:
        big_right = lastStable * 2.5
    elif lastStable >= 20 and lastStable < 35:
        big_right = lastStable * 36.0 - 670
    elif lastStable >= 35 and lastStable < 500:
        big_right = lastStable * 2.25 + 511.25
    elif lastStable >= 500 and lastStable < 8600:
        big_right = lastStable * 2.0 + 636.25
    big_right = max(5, big_right)

    big_left = min(8600, big_left)
    big_right = min(8600, big_right)

    if need_small:
        ### small
        # left
        small_left = big_left*0.6 + curLux*0.4
        # right
        small_right = big_right*0.6 + curLux*0.4
        if curLux > small_right and curLux < big_right:
            return curLux + (big_right-curLux)*0.1
        elif curLux > big_left and curLux < small_left:
            return curLux - (curLux-big_left)*0.1
        elif curLux >= small_left and curLux <= small_right:
            return lastStable
        else:
            return curLux
    else:
        if curLux <= big_right and curLux >= big_left:
            return lastStable
        else:
            return curLux

if __name__ == '__main__':
    # 1,2,3,4,5,6,6,6,7,5,3,8,9
    # 1,2,3,4,5,6,6,6,7,5,3,2,1
    # 1,2,3,4,5,6,6,6,7,5,7,5,7,5,7,5

    lux_list = [50,51,51,52,50,51,51,52,80,130,150,170,200,200,201,202,210,200,200,201,202,210,200,200,201,202,210,200,200,201,202]    
    lux_list = readFromRecord('record2.txt') 
    stable_list, stable_idx = AIBrightness(lux_list, need_small=False, always_usd_std=False)
    print("\norigin:", lux_list)
    print("stable:", stable_list)
    print("stable idx:", stable_idx)

    plt.subplot(2,1,1)
    # draw(lux_list, stable_list, stable_idx)
    X = np.linspace(0, len(lux_list)*0.2, len(lux_list))
    # X = np.arange(0, len(lux_list)*0.2, 0.2)
    plt.plot(X, np.array(lux_list), marker='.')  # , mec='r', mfc='r', markersize=6
    stable_idx = [X[int(idx)] for idx in stable_idx]
    plt.subplot(2,1,2)
    plt.plot(np.array(stable_idx), np.array(stable_list), marker='.')  # , mec='g', mfc='g', markersize=6
    # plt.xticks(X)

    plt.show()
