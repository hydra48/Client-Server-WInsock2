#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")


struct sockaddr_in addr;
CRITICAL_SECTION lock;
std::string msg = "Le développement informatique est une discipline à la fois technique, scientifique, créative et profondément expérimentale dans laquelle un développeur apprend progressivement à transformer une idée abstraite en un système concret capable d'être exécuté par une machine, et cette transformation commence souvent par la compréhension des concepts fondamentaux de la programmation comme les variables, les conditions, les boucles, les fonctions, les structures de données et les algorithmes, avant de conduire vers des sujets beaucoup plus complexes comme la gestion manuelle de la mémoire, les pointeurs, les processus, les threads, la synchronisation, les communications interprocessus, les sockets réseau, les systèmes de fichiers, les mécanismes d'entrée-sortie, les appels système, les interruptions, les niveaux de privilège, les pilotes de périphériques et finalement l'architecture même des systèmes d'exploitation, car derrière une instruction apparemment aussi simple que l'ouverture d'un fichier se cache en réalité une succession impressionnante d'opérations impliquant une bibliothèque utilisateur, une API du système, une transition entre le mode utilisateur et le mode noyau, la validation des paramètres transmis par le programme, la recherche des objets représentant le fichier dans le système, l'intervention éventuelle de plusieurs pilotes, l'accès au système de fichiers, la gestion du cache, la planification des opérations d'entrée-sortie et parfois une communication avec un périphérique physique, ce qui montre à quel point le développement bas niveau permet de découvrir progressivement les différentes couches d'abstraction qui rendent les ordinateurs modernes utilisables, et c'est justement lorsque l'on commence à étudier des langages comme le C et le C++ que cette réalité devient particulièrement visible puisque le programmeur peut manipuler directement des adresses mémoire, comprendre la différence entre la pile et le tas, observer la disposition réelle des structures en mémoire, étudier l'alignement des données, utiliser des pointeurs de fonctions, manipuler des bits, examiner le code assembleur produit par le compilateur et comprendre pourquoi deux morceaux de code qui semblent presque identiques peuvent avoir des comportements ou des performances très différents, tandis que l'apprentissage de l'assembleur complète cette vision en montrant comment les instructions sont réellement exécutées par le processeur, comment fonctionnent les registres, la pile d'appels, les conventions d'appel, les branchements, les opérations arithmétiques et logiques, les accès mémoire et les transitions entre différentes fonctions, ce qui devient extrêmement utile lorsqu'on utilise un débogueur comme WinDbg ou x64dbg pour examiner un programme instruction par instruction, inspecter les registres, poser des points d'arrêt, suivre une chaîne d'appels et comprendre l'origine exacte d'un crash, d'une corruption mémoire ou d'un comportement inattendu, mais être développeur ne consiste évidemment pas uniquement à connaître beaucoup d'API ou à mémoriser des centaines de fonctions, car la compétence essentielle reste la capacité à construire un modèle mental précis du fonctionnement d'un programme, à formuler des hypothèses lorsqu'un problème apparaît, à créer une expérience permettant de vérifier ces hypothèses puis à interpréter correctement les résultats obtenus, et c'est pour cette raison que les petits projets techniques sont souvent extrêmement formateurs : écrire un serveur TCP oblige par exemple à comprendre la création d'un socket, l'association d'une adresse, l'écoute, l'acceptation des connexions, la réception et l'envoi des données, mais une première version fonctionnelle conduit rapidement à de nouvelles questions concernant les lectures partielles, les écritures partielles, la détection correcte des déconnexions, la concurrence entre plusieurs clients, la synchronisation entre threads, les protocoles applicatifs, la gestion des erreurs, les timeouts et éventuellement les mécanismes asynchrones comme les I/O Completion Ports sous Windows, exactement comme l'écriture d'un premier pilote Windows capable de recevoir un IOCTL conduit naturellement à vouloir comprendre ce qu'est réellement un IRP, comment fonctionne la pile de périphériques, pourquoi IoCallDriver transmet une requête au pilote inférieur, pourquoi une completion routine peut être exécutée lorsque cette requête remonte la pile, comment les buffers sont transférés entre le mode utilisateur et le noyau, pourquoi certaines opérations ne sont autorisées qu'à certains IRQL et pourquoi une erreur qui provoquerait simplement le crash d'un programme classique peut provoquer un écran bleu lorsqu'elle est commise dans le noyau, et cette progression illustre une caractéristique fondamentale du développement système : chaque notion comprise révèle généralement une couche supplémentaire qui était auparavant invisible, puisque comprendre les threads mène au scheduler, comprendre le scheduler mène aux priorités et aux changements de contexte, comprendre les changements de contexte mène aux structures internes représentant les processus et les threads, comprendre les entrées-sorties mène aux IRP et aux piles de périphériques, comprendre la mémoire virtuelle mène aux tables de pages, aux protections mémoire, au partage de pages et au fonctionnement du gestionnaire mémoire, tandis que comprendre les exécutables mène naturellement aux formats comme PE sous Windows ou ELF sous Linux, aux sections, aux imports, aux relocations, au chargement dynamique et au rôle du loader, ce qui explique pourquoi le reverse engineering constitue également un excellent complément au développement bas niveau puisqu'il force à regarder les programmes depuis l'autre direction : au lieu de partir du code source pour produire un programme, on part du programme compilé pour reconstruire progressivement son fonctionnement à partir des instructions machine, des chaînes de caractères, des imports, des structures de données et du comportement observé pendant l'exécution, et cette capacité à naviguer entre le code source, les API du système, les structures internes, l'assembleur et le comportement réel de la machine donne progressivement une compréhension beaucoup plus profonde de l'informatique que la simple connaissance syntaxique d'un langage, car connaître C++ ne signifie pas seulement savoir écrire une classe, utiliser std::vector ou appeler une fonction de la bibliothèque standard, mais comprendre ce que ces abstractions produisent réellement, quand elles sont utiles, quels coûts elles introduisent et quand une approche plus directe est préférable, tout comme connaître Windows ne signifie pas seulement savoir appeler CreateProcess, CreateThread, VirtualAlloc, CreateFile ou CreateFileMapping, mais comprendre les objets noyau auxquels ces fonctions donnent accès, la manière dont les handles les référencent, les droits d'accès associés, le rôle du gestionnaire d'objets et la frontière entre les API Win32 documentées et les mécanismes internes du système, et au fil de cette progression le développeur finit souvent par constater que les bugs les plus intéressants ne viennent pas forcément d'une instruction manifestement incorrecte mais d'une hypothèse subtile qui était fausse, comme supposer qu'un appel à send transmet nécessairement l'intégralité d'un buffer, croire qu'un pointeur reste valide après la libération de la zone qu'il référence, oublier qu'un autre thread peut modifier une donnée entre deux instructions, utiliser une ressource paginable depuis un contexte où elle ne peut pas être chargée, conserver un verrou pendant une opération potentiellement bloquante, oublier de vérifier un code de retour ou supposer qu'une structure interne du système possède toujours les mêmes offsets entre plusieurs versions, et c'est précisément cette recherche permanente des hypothèses cachées qui rend le débogage aussi important que l'écriture du code elle-même, parce qu'un bon développeur système doit être capable non seulement de créer un programme lorsque tout fonctionne correctement, mais surtout de déterminer pourquoi il ne fonctionne plus lorsqu'une situation imprévue apparaît, en utilisant les logs, les traces, les dumps mémoire, les débogueurs, les outils d'analyse statique et dynamique, les désassembleurs et parfois simplement une réduction méthodique du problème jusqu'à obtenir le plus petit exemple capable de reproduire l'erreur, tandis que l'étude des systèmes d'exploitation permet de replacer toutes ces connaissances dans un ensemble cohérent où les processus, la mémoire virtuelle, les systèmes de fichiers, le réseau, les pilotes, les mécanismes de sécurité et la concurrence ne sont plus des sujets indépendants mais différentes parties d'une même architecture conçue pour partager efficacement et sûrement les ressources physiques d'une machine entre de nombreux programmes, et c'est finalement cette capacité à passer continuellement de la vision globale à la vision détaillée, d'une architecture complète jusqu'à une instruction assembleur particulière, qui rend le développement informatique aussi vaste et passionnant, puisqu'il est toujours possible d'aller un niveau plus bas, d'observer une abstraction que l'on utilisait auparavant sans la comprendre, de construire un petit programme pour tester son fonctionnement, de provoquer volontairement certaines situations pour observer la réaction du système, de lire la documentation ou le code source disponible pour compléter son modèle mental, puis de revenir à ses propres projets avec une compréhension plus précise de ce que fait réellement la machine, de sorte qu'après des années d'apprentissage le développeur continue encore à découvrir de nouveaux mécanismes, de nouvelles architectures, de nouvelles optimisations et de nouvelles façons de résoudre les problèmes, non pas parce qu'il aurait échoué à tout apprendre auparavant, mais simplement parce que l'informatique moderne est constituée d'une quantité gigantesque de couches qui se sont accumulées au fil des décennies, depuis les transistors et les instructions du processeur jusqu'aux noyaux, aux pilotes, aux bibliothèques système, aux runtimes, aux protocoles réseau et aux applications, et que comprendre progressivement ces couches, apprendre à les manipuler correctement et surtout être capable de raisonner sur leurs interactions constitue l'une des dimensions les plus intéressantes du métier de développeur.";
char* cstr = msg.data();
SOCKET acpt;

DWORD WINAPI receive(LPVOID socket) {
	while (true) {

	}
	return 0;
}

void sendata(SOCKET s, char* data) {
	int total = 0;
	char buffer[4097];
	char buffsize[16] = { 0 };
	int size = strlen(data);
	int size2 = strlen(data);
	sprintf_s(buffsize, sizeof(buffsize), "siz%d", size2);
	std::cout << buffsize << std::endl;
	send(s, buffsize, sizeof(buffsize), 0);
	
	do {
		EnterCriticalSection(&lock);
		int minimumdata = min(4096, size);
		memcpy(buffer, data,minimumdata);
		buffer[minimumdata] = '\0';
		std::cout << "memoire copiee" << std::endl;
		int tosend = min(4096, size);
		int bytesent = send(s, buffer, tosend, 0);
		std::cout << "donnees envoyees : " << bytesent << std::endl;
		total = bytesent + total;
		data = data + bytesent;
		size = size - bytesent;
		std::cout << buffer << std::endl;
		LeaveCriticalSection(&lock);
	} while (total < size2);

	return;

}

int main()
{
	InitializeCriticalSection(&lock);
	WSAData data = { 0 };
	if (WSAStartup(2.2, &data) == 0) {
		std::cout << "succes wsastratup" << std::endl;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7777);
	if (bind(s, (sockaddr*)&addr, sizeof(addr)) == 0) {
		std::cout << "succes bind" << std::endl;

	}
	if (listen(s, 10) == 0) {
		std::cout << "succes listen" << std::endl;

	}
	int size = sizeof(addr);
	while (true) {
		try
		{
			acpt = accept(s, (sockaddr*)&addr, &size);
		    HANDLE thread = CreateThread(NULL, 0, receive, &acpt, 0, NULL);
			std::cin;
			break;
			
			

		}
		catch (const std::exception&)
		{
			std::cout << "socket deja accepte" << std::endl;
		}
		Sleep(1000);
	}
	sendata(acpt, cstr);
	DeleteCriticalSection(&lock);
	


}