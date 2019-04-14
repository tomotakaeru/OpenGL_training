import cv2
import numpy as np

class mouseParam:
    def __init__(self, input_img_name):
        #マウス入力用のパラメータ
        self.mouseEvent = {"x":None, "y":None, "event":None, "flags":None}
        #マウス入力の設定
        cv2.setMouseCallback(input_img_name, self.__CallBackFunc, None)
    
    #コールバック関数
    def __CallBackFunc(self, eventType, x, y, flags, param):
        self.mouseEvent["x"] = x
        self.mouseEvent["y"] = y
        self.mouseEvent["event"] = eventType    
        self.mouseEvent["flags"] = flags    

    #マウスイベントを返す関数
    def getEvent(self):
        return self.mouseEvent["event"]                

    #マウスフラグを返す関数
    def getFlags(self):
        return self.mouseEvent["flags"]                

    #xとyの座標を返す関数
    def getPos(self):
        return [self.mouseEvent["x"], self.mouseEvent["y"]]

def getPos_from_click(img_name, comment):
    print(comment)
    cv2.namedWindow("before", cv2.WINDOW_NORMAL) #windowのサイズを可変にする
    cv2.imshow("before", img_name)
    mouseData = mouseParam("before")
    while 1:
        cv2.waitKey(20)
        if mouseData.getEvent() == cv2.EVENT_LBUTTONDOWN: #左クリックに反応
            pos = mouseData.getPos()
            break;
    cv2.destroyAllWindows()
    print(pos)
    return pos


if __name__ == "__main__":
    img = cv2.imread('../data_before.png')
    h, w, c = img.shape

    left_upper = getPos_from_click(img, "click Left Upper corner")
    left_lower = getPos_from_click(img, "click Left Lower corner")
    right_lower = getPos_from_click(img, "click Right Lower corner")
    right_upper = getPos_from_click(img, "click Right Upper corner")

    src_pts = np.array([left_upper, left_lower, right_lower, right_upper], dtype=np.float32)
    dst_pts = np.array([[0, 0], [0, h], [w, h], [w, 0]], dtype=np.float32)
    mat = cv2.getPerspectiveTransform(src_pts, dst_pts)
    result = cv2.warpPerspective(img, mat, (w, h), flags=cv2.INTER_CUBIC)

    cv2.imwrite("../data_after_py.png", result)
    cv2.namedWindow("after", cv2.WINDOW_NORMAL)
    cv2.imshow("after", result)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
