## Compré-C
Vous allez faire un outil de gestion de fichiers zip.
Votre programme permettra différentes choses :
- Pour les fichiers ZIP protégés, une option pour tenter de les bruteforce dictionnaire. À l'issue du bruteforce, 
le mot de passe trouvé sera affiché.
- On doit aussi être capable de déchiffrer un fichier dans l'archive zip avec un mot de passe donné par 
l'utilisateur (Pour l'extraire par exemple).
- On doit pouvoir explorer l'archive de façon interactive.
- Il doit être possible d'extraire un fichier, qui ira alors dans un dossier du poste qui tourne le programme ou 
bien un dossier choisi par l'utilisateur.
- On doit pouvoir demander à ajouter un fichier dans l'archive zip ouverte, en donnant un chemin vers ce 
fichier.
- Votre programme doit avoir une rubrique "help" qui permet de savoir comment l'utiliser.
Il est **INTERDIT** d'utiliser la commande `system`, sauf si c'est pour clear la console.
Pour traiter les archives zip, vous puvez utiliser [libzip](https://libzip.org/) ou une lib similaire de votre choix.
Pour faciliter l'ergonomie de votre programme, vous devrez utiliser au moins une des librairies suivantes :
- getopt
- ncurses
- GTK
- SDL
Voici un exemple d'input/output avec `getopt`:
Ouvrir l'aide :
```
./zip -h
Usage: ./zip [OPTION]... [FILE]...
Imprimé le : 23/03/24 08:44
Options:
 -h, --help Show this help
 -o, --open Open a zip file for browsing
 -b, --bruteforce Try to bruteforce the password
 -d, --dictionary=FILE Try to bruteforce the password with a dictionary
 -p, --password=PASSWORD Use this password
 -e, --extract=FILE Extract this file
 -i, --include=FILE Include this file
```
Ouvrir un fichier zip avec un mot de passe, et utiliser interactivement le programme :
```
./zip -o --password=password archive.zip
Actions: extract file or directory(e), visit directory(v), open file(o), quit(q)
1 - some_directory
2 - some_file.txt
3 - some_other_file.txt
ex: e1 to extract the first directory
>v1
Actions: extract file or directory(e), visit directory(v), open file(o), quit(q)
1 - some_deep_directory
2 - some_deep_file.txt
3 - some_other_deep_file.txt
```
Bruteforce un fichier zip avec dictionnaire :
```
./zip -b -d dictionary.txt archive.zip
Password found: password
```
Vous serez notés sur les consignes ci-dessus, ainsi que sur:
- La qualité du code
- La qualité de l'interface
- La qualité de l'architecture du projet
- Votre travail de groupe
Les ajouts de fonctionnalités seront appréciés.