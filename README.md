# Final_project
Created on 110.6.12 at home.

# Q1: How to setup and run your program？
A1:  
在OpenMV的鏡頭所灌入的main.py中，我先將曝光率調整為1.25倍，讓螢幕所接收到的光更亮，這樣一來用黑筆所寫出的數字以及AprilTag的辨識將可以更加清楚~ 接著我讓它同時偵測螢幕中的數字以及AprilTag，當偵測到數字(已事先利用鏡頭拍攝每個數字、符號，以及白底背景各約150張照片(有上傳到database的資料夾中)當作機器學習的數據資料，再上傳到線上 "Edge Impulse"網頁進行機器學習，設定參數為96 * 96大小、灰階，以及其他參數以符合OpenMV H5 Plus的記憶體容量。將學習得出的最後結果複製到OpenMV的記憶體中即可)時就透過uART傳資料(單行，只有此數字)給板子；而當偵測到AprilTag時，就以 "ID:??"的形式透過uART將偵測到的AprilTag的ID值傳送給板子。一開始我會先利用car_control.py中按下向右鍵傳送RPC指令，使車子開始行動，而緊隨在RPC指令後的為三個double數值(會輸入最多至小數點後第二位)，分別代表三個選項，其中一個將會是車子啟動之後所偵測到的算式的解答。當RPC指令送達後，車子就會開始以慢速移動，並且同時接收來自OpenMV所傳來的

# Q2: What are the results？
A2:
