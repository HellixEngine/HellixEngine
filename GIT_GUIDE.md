Para criar o arquivo diretamente na sua máquina sem depender do download, abra o terminal na pasta raiz do projeto (`D:\Projetos\C++\HellixEngine`) e crie o `GIT_GUIDE.md` copiando o conteúdo abaixo:

```markdown
# Guia Prático de Git para a Equipe (GIT_GUIDE.md)

Este guia reúne os comandos essenciais do Git para o fluxo de trabalho diário na **HellixEngine**, cobrindo desde a configuração inicial da máquina até a resolução de conflitos e abertura de Pull Requests[cite: 15].

---

## 1. Configuração Inicial (Feito uma única vez por máquina)

Antes de qualquer commit, configure sua identidade para que os registros no histórico fiquem corretos[cite: 15]:

```bash
# Define seu nome e e-mail vinculados à sua conta do GitHub
git config --global user.name "Seu Nome"
git config --global user.email "seu-email@exemplo.com"

# Garante que pull use rebase por padrão (evita commits de merge sujos)
git config --global pull.rebase true

```

---

## 2. Clonando o Repositório e Trocando de Branch

Para começar a trabalhar no projeto pela primeira vez:

```bash
# 1. Clona o repositório para a sua máquina
git clone [https://github.com/SEU_USUARIO/HellixEngine.git](https://github.com/SEU_USUARIO/HellixEngine.git)

# 2. Entra na pasta do projeto
cd HellixEngine

# 3. Lista todas as branches disponíveis (locais e remotas)
git branch -a

# 4. Muda para a branch da sua frente de trabalho (exemplo: renderer)
# O Git automaticamente rastreia a branch remota correspondente
git checkout feature/renderer

```

---

## 3. O Ciclo Diário de Trabalho

Durante o desenvolvimento do seu módulo, o fluxo segue 4 etapas: inspecionar, preparar, registrar e sincronizar.

### 3.1 Checar o que foi alterado

```bash
# Mostra arquivos modificados, novos ou excluídos
git status

# Mostra exatamente as linhas que mudaram no código antes de preparar o commit
git diff

```

### 3.2 Preparar os arquivos para commit (Staging Area)

```bash
# Adiciona um arquivo específico para a área de preparação
git add engine/src/renderer/shader.cpp

# Adiciona todos os arquivos modificados da pasta atual
git add .

```

### 3.3 Gravar as alterações (Commit)

Siga o padrão de **Conventional Commits** definido no projeto:

```bash
# Estrutura: tipo(escopo): mensagem curta no imperativo
git commit -m "feat(renderer): implement basic vertex array layout"

# Outros exemplos:
git commit -m "fix(window): resolve vsync toggle glitch on linux"
git commit -m "refactor(core): encapsulate raw opengl handle"

```

### 3.4 Enviar para o repositório remoto (Push)

```bash
# Envia os commits da sua branch local para a branch remota correspondente
git push origin feature/renderer

```

---

## 4. Mantendo sua Branch Atualizada com a Main (Rebase)

Conforme outros membros mesclam código na `main`, a sua branch precisa ser atualizada para evitar conflitos no futuro. **Nunca use `git merge main` dentro da sua feature branch**; utilize `rebase`:

```bash
# 1. Garanta que você não deixou arquivos modificados sem commit
git status

# 2. Atualize sua branch main local
git checkout main
git pull origin main

# 3. Volte para a sua feature branch
git checkout feature/renderer

# 4. Aplique seus commits em cima do código mais recente da main
git rebase main

```

### O que fazer se houver conflito durante o Rebase?

* O Git pausa o processo e indica quais arquivos estão com conflito.


* Abra os arquivos conflitantes no CLion ou editor, escolha as alterações corretas e salve.


* Marque o arquivo resolvido com `git add <caminho-do-arquivo>`.


* Continue o processo rodando:


```bash
git rebase --continue

```


* Se algo der errado e quiser cancelar tudo com segurança:


```bash
git rebase --abort

```


* Após concluir um rebase com sucesso, o envio para o repositório remoto exigirá lease forçado:


```bash
git push --force-with-lease origin feature/renderer

```



---

## 5. Lidando com Imprevistos e Desfazendo Ações

### Descartar alterações que ainda não foram salvas

```bash
# Descarta todas as alterações não commitadas em um arquivo específico
git checkout -- caminho/do/arquivo.cpp

# Descarta TODAS as alterações não commitadas no projeto inteiro
git restore .

```

### Guardar código temporariamente (Stash)

Para puxar atualizações sem perder alterações em andamento:

```bash
# Guarda as alterações temporariamente na gaveta
git stash

# Puxa as atualizações necessárias
git pull origin main

# Retira as alterações da gaveta de volta para o código
git stash pop

```

### Alterar a mensagem do último commit local

```bash
git commit --amend -m "feat(renderer): nova mensagem correta"

```

---

## 6. Checklist de Abertura de Pull Request (PR)

Antes de pedir a revisão do seu código para a branch `main`:

* [ ] A branch compila limpa localmente sem quebrar outros módulos.


* [ ] Você executou `git checkout main && git pull` e fez `git rebase main` na sua branch.


* [ ] Todos os novos arquivos criados estão listados no `CMakeLists.txt`.


* [ ] Você enviou todas as alterações via `git push origin feature/<nome-da-branch>`.


* [ ] No GitHub, abra o PR com uma descrição clara do que foi implementado e dos testes realizados.


