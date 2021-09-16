<!DOCTYPE html>
<html>
<head>
  <title>ESP32-CAM Galeria de Fotos</title>
 
  <script type="text/javascript">
    
    function actualizar(){location.reload(true);}
    
    //Función para actualizar cada 2 segundos(2000 milisegundos)
    setInterval("actualizar()",4000);
    
  </script>

</head>
<body>
  
  <div class="titulos">
    <h1>Sistema de Proximidad</h1>
    <h2>ESP32-CAM Galeria de Fotos</h2>
  </div>
<a href="index1.html" class="btn-flotante">Inicio</a>
<?php
  // Extensiones de imagenes
  $image_extensions = array("png","jpg","jpeg","gif");

  // Comprobamos elminado HTTP GET request - remove images
  if(isset($_GET["delete"])){
    $imageFileType = strtolower(pathinfo($_GET["delete"],PATHINFO_EXTENSION));
    if (file_exists($_GET["delete"]) && ($imageFileType == "jpg" ||  $imageFileType == "png" ||  $imageFileType == "jpeg") ) {
      echo "Archivo encontrado y eliminado: " .  $_GET["delete"];
      unlink($_GET["delete"]);
    }
    else {
      echo 'Archivo no encontrado - <a href="galeria.php">refresh</a>';
    }
  }
  
  $dir = 'subidas/';
  if (is_dir($dir)){
    foreach ($files as $file) {
      if ($file != '.' && $file != '..') {?>
        <div>
          <hr>
          <div class="a">
             <p><a href="galeria.php?delete=<?php echo $dir . $file; ?>" >Eliminar foto</a>  <?php echo $file; ?></p><br>
           </div>
           <a href="<?php echo $dir . $file; ?>">
            <img src="<?php echo $dir . $file; ?>"/>
           </a>
      
          
        </hr><br>
        <hr></hr>
       </div>
<?php
       $count++;
      }
    }
  }
  if($count==1) { echo "<p>Imagenes no encontradas</p>"; } 
?>


 
</body>
</html>
