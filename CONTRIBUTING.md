# Guia de Contribuição e Diretrizes de Engenharia (CONTRIBUTING.md)

Este documento estabelece as diretrizes de desenvolvimento, arquitetura, convenções de código, versionamento com Git e políticas de uso de ferramentas de automação/IA para o projeto da Game Engine. O objetivo é manter a consistência do código, facilitar a revisão (code review) e garantir o aprendizado técnico real de todos os integrantes.

---

## 1. Arquitetura do Repositório e Diretórios

O projeto segue uma separação modular estrita entre a **Engine** (biblioteca modular reutilizável), a **Sandbox** (aplicação executável de teste) e os módulos de suporte/documentação.

```text
projeto-engine/
├── CMakeLists.txt             # Configuração raiz do build
├── CONTRIBUTING.md            # Este arquivo de governança
├── README.md                  # Apresentação do projeto e instruções de build
├── docs/                      # Especificações de arquitetura e relatórios técnicos
│   └── architecture/
├── engine/                    # Código-fonte da Engine (biblioteca)
│   ├── CMakeLists.txt
│   ├── include/               # Interfaces e contratos públicos (.hpp / .h)
│   │   └── engine/
│   │       ├── core/          # Application, Window, Logger, Timestep
│   │       ├── events/        # Sistema de eventos e tratamento de input
│   │       ├── renderer/      # Interfaces de Renderização, Shaders, Buffers, Texturas
│   │       └── scene/         # Entidades, componentes e hierarquia de cena
│   └── src/                   # Implementações privadas (.cpp / .c)
│       ├── core/
│       ├── renderer/
│       └── platform/          # Código acoplado a SO ou APIs gráficas (ex: opengl/, win32/)
├── sandbox/                   # Aplicação/Jogo para validar as funcionalidades da engine
│   ├── CMakeLists.txt
│   └── src/
│       └── main.cpp
└── vendor/                    # Dependências de terceiros (submódulos Git ou bibliotecas externas)
```

### Regras de Organização de Arquivos
1. **Cabeçalhos Públicos (`engine/include/engine/`):** Contêm apenas declarações de classes, structs, enums e assinaturas de funções públicas. **Nunca** inclua cabeçalhos proprietários de plataformas (ex: `windows.h`, `glad/glad.h`, `GL/gl.h`) dentro dos headers públicos.
2. **Implementações Privadas (`engine/src/`):** Todo detalhe de implementação e chamadas a APIs nativas ficam restritos à pasta `src` ou `src/platform`.
3. **Guarda de Inclusão:** Todo arquivo `.h` ou `.hpp` deve iniciar com `#pragma once` na primeira linha.
4. **Dependências do CMake:** Sempre que um novo arquivo `.c` ou `.cpp` for criado, ele deve ser registrado no `CMakeLists.txt` do respectivo módulo.

---

## 2. Convenções de Código e Naming Conventions

Para garantir legibilidade uniforme em toda a base de código, adotamos o seguinte padrão:

| Elemento | Padrão | Exemplo |
| :--- | :--- | :--- |
| **Arquivos e Pastas** | `snake_case` | `render_command.hpp`, `vertex_array.cpp` |
| **Classes, Structs e Types** | `PascalCase` | `class VertexBuffer;`, `struct KeyEvent;` |
| **Métodos e Funções** | `camelCase` | `void drawIndexed();`, `bool pollEvents();` |
| **Variáveis Locais e Argumentos** | `snake_case` | `float delta_time;`, `uint32_t buffer_size;` |
| **Membros Privados / Protegidos** | Prefixo `m_` + `camelCase` | `uint32_t m_rendererID;`, `bool m_isRunning;` |
| **Constantes e Enum Values** | `UPPER_SNAKE_CASE` | `MAX_BATCH_QUADS`, `KEY_STATE_PRESSED` |
| **Namespaces** | `snake_case` | `namespace engine::renderer { ... }` |

### Boas Práticas Obrigatórias
* **Proibição de `using namespace std;` em Headers:** É expressamente proibido incluir `using namespace ...;` em qualquer arquivo `.h` ou `.hpp`. Em arquivos `.cpp`, dê preferência ao escopo explícito (`std::string`, `std::vector`).
* **Headers Auto-contidos:** Qualquer arquivo de cabeçalho deve compilar isoladamente. Se ele usa `std::string`, deve obrigatoriamente incluir `<string>`.
* **Gerenciamento de Recursos (RAII):** Em C++, evite ponteiros brutos (`new`/`delete`) para gerenciar vida útil de objetos. Utilize `std::unique_ptr` ou `std::shared_ptr`. Recursos de GPU (VBO, VAO, Textures) devem ser alocados no construtor e liberados no destrutor.

---

## 3. Padrão de Comentários e Documentação

* **O "Porquê", não o "O quê":** Não comente trechos óbvios. Explique suposições matemáticas, workarounds de drivers, decisões de design de memória ou alinhamento de dados.
* **Documentação de Interfaces Públicas (Estilo Doxygen):** Todas as classes e métodos expostos na API pública da Engine devem conter anotações estruturadas:

```cpp
/**
 * @brief Envia os comandos de renderização de um quad 2D para o pipeline.
 * 
 * @param position Coordenadas mundiais 2D do centro do objeto.
 * @param size Dimensões (largura, altura) em unidades de mundo.
 * @param color Cor RGBA normalizada no intervalo [0.0f, 1.0f].
 */
void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
```

---

## 4. Fluxo Git, Estratégia de Branches e Commits

A estabilidade da branch principal é responsabilidade coletiva, gerida pelo Maintainer do projeto.

### 4.1 Estratégia de Branches
* **`main`:** Código estável, compilável e testado. É estritamente proibido fazer `push` direto na `main`. Todo código entra via Pull Request (PR).
* **Branches de Desenvolvimento:** Devem ser ramificadas a partir da `main` e seguir a nomenclatura:
  * `feature/<nome-descritivo>`: Novas funcionalidades (ex: `feature/gl-batch-renderer`, `feature/window-events`).
  * `fix/<nome-descritivo>`: Correções de bugs ou vazamentos de memória (ex: `fix/input-key-repeat-bug`).
  * `refactor/<nome-descritivo>`: Refatorações estruturais sem alteração de comportamento externo.

### 4.2 Rebase Obrigatório Antes de Abrir PR
Antes de submeter ou solicitar a revisão de um Pull Request, a branch deve estar atualizada com a `main`:
```bash
git checkout main
git pull origin main
git checkout feature/meu-modulo
git rebase main
# Resolva eventuais conflitos localmente e faça o push com --force-with-lease se necessário
```

### 4.3 Padrão de Commits (Conventional Commits)
As mensagens de commit devem ser objetivas, escritas em inglês ou português (mantenha a consistência no PR) e seguir a estrutura:

```text
tipo(escopo): mensagem curta e clara no imperativo
```

* **Tipos permitidos:**
  * `feat`: Nova funcionalidade adicionada.
  * `fix`: Correção de falha ou bug.
  * `refactor`: Alteração de código que não corrige bug nem adiciona funcionalidade.
  * `build`: Alterações que afetam o sistema de build (`CMakeLists.txt`, dependências externas).
  * `docs`: Inclusão ou modificação de documentação e diagramas.
  * `perf`: Melhoria de desempenho.
* **Exemplos válidos:**
  * `feat(renderer): implement basic vertex buffer abstraction`
  * `fix(events): prevent segfault when window close event has no listener`
  * `build(cmake): link glad and glfw statically on linux target`

---

## 5. Política de Uso de Ferramentas de Inteligência Artificial (IA)

O objetivo central deste projeto é a capacitação técnica profunda dos integrantes em engenharia de software de baixo nível, arquitetura de sistemas e computação gráfica. Dessa forma, aplicam-se as seguintes restrições:

### 5.1 O que é Permitido ✅
* **Tirar Dúvidas Conceituais e Sintáticas:** Consultar LLMs para esclarecer funcionamento de especificações (ex: "como funciona o layout `std140` de uniform buffers no OpenGL?").
* **Depuração de Erros Complexos:** Analisar logs crípticos de compiladores (`gcc`, `clang`, `msvc`) ou mensagens do linker (`undefined reference`).
* **Geração de Documentação e Relatórios:** Estruturar relatórios técnicos, minutas e diagramas conceituais em Markdown.
* **Brainstorming de Arquitetura:** Avaliar prós e contras entre padrões de projeto (ex: ECS vs. OOP tradicional para o subsistema de cena).

### 5.2 O que é Proibido ❌
* **Uso de Agentes Autônomos de Código:** É expressamente vetado o uso de agentes que analisam o repositório e geram módulos, classes ou subsistemas inteiros de forma autônoma.
* **"Vibe Coding" / Colar Código Desconhecido:** Nenhum membro deve submeter código que não compreenda integralmente.

### 5.3 Regra de Ouro da Revisão de Código (Code Review)
O autor do Pull Request é **100% responsável por cada linha submetida**. 
Durante a revisão com o Maintainer, caso o autor não saiba explicar o porquê de uma alocação, a mecânica de um ponteiro, ou o fluxo de execução de um trecho de código, o **Pull Request será rejeitado sumariamente** para reescrita manual.

---

## 6. Checklist para Aprovação de Pull Request (PR)

Antes de solicitar o merge ao Maintainer, certifique-se de que o seu PR cumpre todos os requisitos:

- [ ] A branch compila sem erros ou warnings críticos em todos os compiladores suportados.
- [ ] Nenhum arquivo `.h` ou `.hpp` contém `using namespace std;`.
- [ ] Todos os novos arquivos foram devidamente incluídos no `CMakeLists.txt`.
- [ ] Foi realizado o `git rebase main` e não há conflitos pendentes.
- [ ] A aplicação de teste (`sandbox`) roda a funcionalidade sem crashes ou memory leaks aparentes.
- [ ] O autor está apto a explicar tecnicamente todas as linhas alteradas.
