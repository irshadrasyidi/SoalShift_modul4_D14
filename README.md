# SoalShift_modul4_D14
## Soal 1:
- Disiapkan 2 fungsi enkripsi dan dekripsi dengan cara caesar chiper dengan key 17
- Sebelum dijalankan FUSE-nya, saya mengenkrip dulu direktori `/home/irshadrasyidi/shift4` terlebih dahulu di awal2 fungsi main
- Proses enkripsi secara rekursif dengan fungsi `listDir()` supaya yang terenkrip adalah semua folder dan subfolder beserta semua file di dalamnya
- Lalu, saat dijalankan FUSE-nya, digunakan 3 fungsi yaitu:
  - xmp_getattr
  - xmp_readdir
  - xmp_read
- Di fungsi ini, diselipkan fungsi dekrip di dalamnya supaya dapat ditampilkan dengan nama2 normal (nama2 sebelum dienkripsi)

## Soal 2, 3, 4, 5:
- Belum selesai :(
- Karena terlalu lama saat mengerjakan fungsi `listDir()` untuk nomor 1, karena bingung bagaimana mengkombinasi proses rekursi yang masuk ke dalam subfolder2 dengan tetap memperhatikan proses enkripsi namanya
- Juga sempat bingung lama dimana menyelipkan fungsi dekripnya waktu di 3 fungsi untuk FUSE

# Revisi
## Soal 3:
- Digunakan 2 library #include <pwd.h> dan #include <grp.h> untuk ambil username dan group.
- Digunakan juga library #include <time.h> untuk mengambil waktu sistem (untuk pencatatan ke filemiris.txt)
- Pada fungsi fuse xmp_readdir, tambahkan beberapa `if` untuk mengecek username, group, serta filenya harus tidak bisa dibaca
- Jika kriteria `if` berikut terpenuhi, maka catat informasi2 file tersebut ke sebuah string yang kemudian dicatatkan ke filemiris.txt
- Lalu, di bagian akhir `if`, hapus file tersebut dengan fungsi `remove()`
