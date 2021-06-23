# Final_project
Created on 110.6.12 at home.

# Q1: How to setup and run your program？
A1:  
在OpenMV的鏡頭所灌入的main.py中，我先將曝光率調整為1.25倍，讓螢幕所接收到的光更亮，這樣一來用黑筆所寫出的數字以及AprilTag的辨識將可以更加清楚~ 接著我讓它同時偵測螢幕中的數字以及AprilTag，當偵測到數字(已事先利用鏡頭拍攝每個數字、符號，以及白底背景各約150張照片(有上傳到database的資料夾中)當作機器學習的數據資料，再上傳到線上 "Edge Impulse"網頁進行機器學習，設定參數為96 * 96大小、灰階，以及其他參數以符合OpenMV H5 Plus的記憶體容量。將學習得出的最後結果複製到OpenMV的記憶體中即可)時就透過uART傳資料(單行，只有此數字)給板子；而當偵測到AprilTag時，就以 "ID:??"的形式透過uART將偵測到的AprilTag的ID值傳送給板子。一開始我會先利用car_control.py中按下向右鍵透過Xbee傳送RPC指令，使車子開始行動，而緊隨在RPC指令後的為三個double數值(會輸入最多至小數點後第二位)，分別代表三個選項，其中一個將會是車子啟動之後所偵測到的算式的解答。當RPC指令送達後，車子就會開始以慢速移動，並且同時接收來自OpenMV所傳來的算式(照順序放在地上並將鏡頭往地面看即可一一辨識出來)，同時又以ping來偵測與前方障礙物的距離。當OpenMV偵測完算式(共三個字元)後，且ping所偵測到的值小於30(設定值)時，車子就會迅速繞過障礙物，直到走到原路徑的直線延伸方向繼續前進，這時要開始依序讀取/偵測出AprilTag的ID值，因此將車子的前進速率調低以免OpenMV偵測不到。OpenMV會以uART將AprilTag的ID值傳給板子，而板子將算式運算過後所得結果，比照這個ID所對應的數字(之前隨著RPC指令一起傳送到板子而儲存起來的那三個數字依順序對照ID值)是否相同，若不同則繼續前進前往下一個AprilTag；若相等則將車子停下來。這樣即可知道板子是否成功且正確地運算。以上就是我的Final project給BB car的任務(task)。

# Q2: What are the results？
A2:  
一開始車子停止，等著接收由Xbee傳來的RPC指令，伴隨著三個答案選項，當成功接收後，車子開始動，並且接收OpenMV傳來的算式並儲存起來，同時使用ping偵測車子與前方障礙物的距離。當距離小於ping的30單位時，車子轉向並繞過障礙物，回到原直線方向，並且前進到OpenMV偵測到的AprilTag的ID值所對應到的答案選項的值與算式所計算出來的值相同時，車子即停止。 
  
以下為成功完成任務的過程之影片連結：
1. https://www.youtube.com/watch?v=RVI7jdQjx7k
2. https://www.youtube.com/watch?v=Xe2ipEL5ezQ
