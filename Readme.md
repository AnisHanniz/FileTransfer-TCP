# Serveur et Client de Transmission de Fichiers via des Sockets TCP

Ce projet comprend un serveur et un client en langage C qui permettent la transmission de fichiers via des sockets TCP. Le serveur accepte les connexions des clients, récupère les fichiers demandés, puis les transmet aux clients. Le client se connecte au serveur, spécifie la taille de la fenêtre de transmission, et demande le téléchargement d'un fichier spécifique.

## Prérequis

- Système d'exploitation compatible avec les appels système de socket (testé sur Linux).
- Compilateur C (tel que gcc).

## Installation

1. Clonez ce dépôt sur votre machine locale :

   ```
   git clone https://github.com/votre_nom_utilisateur/FileTransferTCP.git
   ```

2. Accédez au répertoire du projet :

   ```
   cd ProjetTransmissionFichiers
   ```

3. Compilez les programmes en utilisant le makefile fourni :

   ```
   make
   ```

## Utilisation

### Serveur

- Exécutez le serveur en spécifiant le répertoire où se trouvent les fichiers à transmettre comme argument en ligne de commande :

   ```
   ./serveur /chemin/vers/repertoire
   ```

   Assurez-vous d'avoir les privilèges nécessaires pour ouvrir et lire les fichiers dans ce répertoire.

### Client

- Exécutez le client en spécifiant la taille de la fenêtre de transmission et le nom du fichier à télécharger comme arguments en ligne de commande :

   ```
   ./client <taille_fenetre> <nom_fichier>
   ```

   Assurez-vous que le serveur est en cours d'exécution et accessible depuis le client.

## Fonctionnalités

### Serveur

- Accepte les connexions des clients.
- Récupère les fichiers demandés par les clients.
- Transmet les fichiers aux clients.

### Client

- Se connecte au serveur.
- Spécifie la taille de la fenêtre de transmission.
- Demande le téléchargement d'un fichier spécifique depuis le serveur.

## Auteurs

Ce programme a été réalisé par [AnisHanniz](https://github.com/AnisHanniz).

