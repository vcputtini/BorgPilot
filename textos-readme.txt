O borgbackup (borg) é um fantastico programa para backup profissional, muito confiável e estável, o qual utilizo a anos.
Apesar do borgbackup ser muito simples de usar na linha e comando e facilmente intregrável a bash scripts, considerei um ótimo exercício criar uma interface gráfica para ele.
A ideia básica é permitir que as principais tarefas possam ser executadas sem que o operador tenha que dispender muito tempo na linha de comando e com scripts.

O BorgPilot executa as seguintes tarefas:

- Permite a inicialização de repositorios locais e remotos (as chaves para conexão remota para ssh sem senha devem estar corretamente configuradas)
- Permite a criação de backups. Via interface o usuário irá definir as características dos backup, tais como: arquivos de origem, opções comuns e opções opcionais e seus valores.
- Listagem e Pesquisa: Permite listar o conteudo dos repositórios de maneira simples, permitindo que sejam usados patterns padrão do borgbackup.
- Utilitades: Permite a execução dos comandos: info, list, check, diff, rename, delete e prune, mostrando os resultados na interface e permitindo
a gravação em arquivos dos resultados.
- Configurações do borgbackup: Diretórios e Arquivos, Gerais e Respostas automáticas.

Nas funções de inicialização e backup dos repositorios, o BorgPilot irá gerar um script bash para que o operador possa executar estas operações,
sendo que, se achar necessário, fazer mudanças no script gerado. Para ver exemplos: src/doc/scripts-sample.

-----
Although borgbackup is very simple to use on the command line and easily integrates with bash scripts, I thought it would be a great exercise to create a graphical interface for it.
The basic idea is to allow the main tasks to be performed without the operator having to spend a lot of time on the command line and with scripts.

BorgPilot performs the following tasks:

- Allows the initialization of local and remote repositories (passwordless SSH remote connection keys must be correctly configured)
- Allows the creation of backups. Through the interface, the user can define the backup characteristics, such as: source files, common options, and optional options, and their values.
- List and Search: Allows for listing the contents of the repositories in a simple manner, allowing the use of standard borgbackup patterns.
- Utilities: Allows the execution of the commands: info, list, check, diff, rename, delete, and prune, displaying the results in the interface and allowing
the results to be saved to files. - BorgBackup Settings: Directories and Files, General, and Automatic Responses.

For repository initialization and backup functions, BorgPilot will generate a bash script for the operator to perform.
You can make changes to the generated script if necessary. For examples, see src/doc/scripts-sample.

==============================
