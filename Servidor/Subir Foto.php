<?php

$target_dir = "subidas/";
$datum = mktime(date('H')+0, date('i'), date('s'), date('m'), date('d'), date('y'));

// Comprobamos si el archivo de la imagen es actual o falsa
if(isset($_POST["submit"])) {
  if($check !== false) {
    echo "El archivo es una imagen - " . $check["mime"] . ".";
    $uploadOk = 1;
  }
  else {
    echo "El archivo no es una imagen.";
    $uploadOk = 0;
  }
}

// Comprobamos si el archivo existe
if (file_exists($target_file)) {
  echo "Losiento, archivo ya existente.";
  $uploadOk = 0;
}

// Comprobamos el tamaño del archivo
if ($_FILES["imageFile"]["size"] > 500000) {
  echo "Losiento, su archivo es muy pesado.";
  $uploadOk = 0;
}

// Todos las extensiones para los archivos
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
  echo "Losiento, solo JPG, JPEG, PNG & GIF son archivos permitidos.";
  $uploadOk = 0;
}

// Comprobamos si $uploadOk envía un 0 o un error
if ($uploadOk == 0) {
  echo "Losiento, su archivo no pudo subirse.";
// Si todo esta k, intenta subir el archivo
}
else {
  if (move_uploaded_file($_FILES["imageFile"]["tmp_name"], $target_file)) {
    echo "El archivo ". basename( $_FILES["imageFile"]["name"]). " ha sido subido.";
  }
  else {
    echo "Losiento, hubo un error al subir su archivo.";
  }
}
?>
