# Tetris SDL2
```
Bài tập môn LTNC INT2215 1
Hà Sơn Tùng - MSV: 21020398
```

## *Cài đặt*
```
- Tải file .exe về 
- Cài đặt mingw và dán các file .dll trong thư mục bin vào  "C:\Windows\System32" và file .dll trong File exe
- Mở file .exe để chơi 
```
## *Mô tả trò chơi* 
### *1. Giới thiệu*
- Tetris là trò chơi xếp các khối gạch, người chơi sẽ cố gắng xếp đầy các viên gạch vào 1 hàng để xóa chúng đi.
- Trò chơi không có thắng thua mà chỉ tính điểm của người chơi.
### *2. Cách chơi*
- Mục tiêu của trò chơi là di chuyển các khối gạch đang rơi từ từ xuống trong kích thước hình chữ nhật 20 hàng x 10 cột (trên màn hình). Chỗ nào có gạch rồi thì không di chuyển được tới vị trí đó. Người chơi xếp những khối hình sao cho khối hình lấp đầy 1 hàng ngang để ghi điểm và hàng ngang ấy sẽ biến mất.
- Nếu để cho những khối hình cao quá màn hình, trò chơi sẽ kết thúc.
- Trò chơi kết thúc khi khối gạch không rơi xuống được nữa.
- Tất cả các Tetriminos có khả năng hoàn thành một và hai dòng. J, L có thể có ba. Chỉ có Tetrimino chữ I có khả năng để xóa bốn dòng cùng một lúc, và điều này được gọi là một "Tetris". Xóa nhiều nhất chỉ được 4 hàng/1 lần.

### *3. Phím tắt*
- Xoay khối: phím mũi tên lên ↑
- Di chuyển sang phải: phím mũi tên phải →
- Di chuyển sang trái: phím mũi tên trái ←
- Làm cho khối gạch rơi nhanh hơn: phím mũi tên xuống ↓
- Giữ 1 khối gạch để dùng sau: phím Enter

### *Hình ảnh minh họa*
![Screenshot (261)](https://user-images.githubusercontent.com/96463130/169630764-3098010a-3fe5-441a-92f3-72c2f27b4af1.png)
![In game](https://user-images.githubusercontent.com/96463130/169630847-7d535934-c3de-4bec-8a29-6fc295e295c9.png)

### *Nguồn lấy ảnh và nhạc*
- https://tetris.com/

## *Các chức năng đã cài đặt*
- Logic game: 
    ```
    Xóa các khối sau khi đã xếp đầy 1 dòng
    Xoay các khối hình
    Có bóng gạch (shadow piece) giúp người chơi biết chính xác địa điểm rơi xuống của khối gạch
    Dự đoán khối gạch tiếp theo
    Giữ khối gạch 
    Tính và lưu điểm cao nhất
    ```
- Hiệu ứng hình ảnh, âm thanh
    
    Âm thanh: 
    ```
      Có nhạc nền, có hiệu ứng âm thanh khi xoay, di chuyển, khóa khối gạch hoặc khối gạch rơi
    ```
    Hình ảnh:
    ```
      Có hiệu ứng hình ảnh khi khóa khối gạch, xóa 1 dòng
    ```
- 1 số tính năng khác:
    ```
    Nút tắt, bật âm thanh
    Chỉnh level cho game khó hơn
    Có thể pause game, thoát game
    ```
- Video demo: https://www.youtube.com/watch?v=U5UMr7ecd3o 
## *Các kỹ thuật sử dụng*
- Render hình ảnh, phát âm thanh, quản lý chuột, bàn phím, thời gian (SDL)
- Sử dụng class, pointer, random, mảng ...
- Phân chia các object thành từng file để dàng vận hành và quản lý
- Thuật toán xoay khối gạch, xử lý va chạm,...
- Tách phần logic và render ra riêng

## *Kết luận, hướng phát triển*
- Việc phân chia object ra thành từng class trong lập trình rất quan trọng vì với cách làm này, ta có thể dễ dàng hơn trong việc lên ý tưởng 
cũng như sửa nếu có lỗi không mong muốn xảy ra. Ngoài ra, nó còn giúp người đọc code dễ dàng nhận ra chương trình của mình đang làm gì.
- Refactor và clean code là 1 điều rất cần thiết sau khi đã hoàn thành chương trình.
- Hướng phát triển:
  ``` 
  Cải tiến thuật toán xoay khối gạch đề nó không bị hạn chế bởi va chạm.
  Làm cho hình ảnh trong game đẹp hơn
  
  ```  
  
    
    



