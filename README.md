# exercices_kernel


## How to run
Pour lancer la vm il faut faire `./run ./bzImage ./initramfs.cpio.gz`  
Le dossier `/tmp` de l'hôte est automatiquement partagé avec la vm. Le point de montage dans la vm est `/tmp/mount` 

**bzImage** est le noyau Linux et **initramfs.cpio.gz** est le système de fichier de la vm.  
Si vous avez besoin de modifier un fichier, il faut lancer `./decompress`, modifier le fichier et `./compress`

## Configuration initramfs
Le fichier que vous voudrez certainement modifier est `etc/init.d/rcS`, c'est le script executé au démarrage. C'est la que beaucoup de configuration du système se passe.
