# exercices_kernel

## Dépendances
Vous allez avoir besoin de qemu et gcc, pour les intaller:  
Ubuntu/Debian based  
`sudo apt install gcc qemu-system-x86`

Arch based  
`pacman -S gcc qemu-system-x86`

Fedora based  
`sudo dnf install gcc qemu`

Nixos based  
`nix-shell -p gcc qemu`

## How to run
Pour lancer la vm il faut faire `./run ./bzImage ./initramfs.cpio.gz`  
Le dossier `/tmp` de l'hôte est automatiquement partagé avec la vm. Le point de montage dans la vm est `/tmp/mount` 

**bzImage** est le noyau Linux et **initramfs.cpio.gz** est le système de fichier de la vm.  
Si vous avez besoin de modifier un fichier, il faut lancer `./decompress`, modifier le fichier et `./compress`

## Configuration initramfs
Le fichier que vous voudrez certainement modifier est `etc/init.d/rcS`, c'est le script executé au démarrage. C'est la que beaucoup de configuration du système se passe.

## How to compile a linux kernel
Vous pouvez regarder les dernières version de kernel linux [ici](https://www.kernel.org/)

Prérequis:  
`sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison`

Pour compiler un noyau minimal comme pour l'example de ce workshop  
```bash
export LINUX_VERSION="6.5.10"
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-$LINUX_VERSION.tar.xz
tar xvf linux-$LINUX_VERSION.tar.xz
cd $LINUX_VERSION
make defconfig
make kvm_guest.config
make -j`nproc`
```

Ensuite, pour compiler un module vous pouvez vous réferrer au Makefile des challenges
