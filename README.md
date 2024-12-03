# 🌐 Webserv - Serveur HTTP/1.1

Un serveur web en C++98 implémentant les fonctionnalités essentielles du protocole HTTP/1.1.

## 🛠️ Fonctionnalités Principales

### 🔄 Gestion des Requêtes
| Méthode | Description |
|---------|-------------|
| GET | Récupération de ressources |
| POST | Envoi de données |
| DELETE | Suppression de ressources |

### ⚙️ Configuration Avancée
- Ports et hosts multiples
- Server names personnalisables
- Pages d'erreur configurables
- Limites de taille de body
- Routes personnalisables
- Redirections HTTP
- Directory listing
- Upload de fichiers

### 🚀 Performance
- I/O non bloquant (poll/select/epoll/kqueue)
- Gestion simultanée de multiples clients
- Zero memory leaks
- Pas de blocage serveur

### 🔌 Support CGI
- Exécution de scripts PHP/Python
- Gestion des variables d'environnement
- Support POST/GET
- Gestion des uploads

## 📋 Spécifications Techniques

### Contraintes
- Standard C++98
- Un seul poll()/select() pour toutes les I/O
- Gestion propre des erreurs
- Compatible navigateurs modernes

## 🌟 Bonus
- Support cookies et sessions
- Gestion multi-CGI
- Stress tests avancés

---
*Projet réalisé dans le cadre du cursus de l'école 42* 🚀
