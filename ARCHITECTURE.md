# Arquitetura e Design Técnico da Engine (ARCHITECTURE.md)

Este documento descreve a topologia estrutural, o fluxo de controle, os contratos de isolamento entre subsistemas e o roadmap da camada de ferramentas para o projeto. Ele serve como especificação formal para garantir que o desenvolvimento paralelo preserve o desacoplamento do núcleo da engine.

---

## 1. Topologia de Diretórios e Fronteiras de Módulos

A organização do código impõe regras rígidas de dependência unidirecional: a biblioteca central (`engine`) desconhece totalmente os executáveis (`sandbox` e `editor`). Dependências de terceiros ficam estritamente confinadas em `vendor/`.

```text
meu-projeto/
├── CMakeLists.txt             # Target raiz: orquestra engine, vendor, sandbox e editor
├── ARCHITECTURE.md            # Este documento de especificação técnica
├── CONTRIBUTING.md            # Regras de governança de código e Git
├── docs/                      # Especificações de subsistemas e relatórios técnicos
│   └── architecture/
├── engine/                    # Camada do Motor (Biblioteca Estática ou Dinâmica)
│   ├── CMakeLists.txt
│   ├── include/               # Contratos públicos consumíveis por clientes externos
│   │   └── engine/
│   │       ├── core/          # EntryPoint, Application, Window, Timestep, Logger, Assert
│   │       ├── events/        # Sistema desacoplado de despacho e pooling de eventos
│   │       ├── renderer/      # Contratos agnósticos: RendererAPI, Buffer, Shader, Texture
│   │       └── debug/         # Hooks opcionais de profiling e instrumentação
│   └── src/                   # Implementação interna e segregação de plataforma
│       ├── core/
│       ├── events/
│       ├── renderer/          # Batch renderer, pipeline de comandos gráficos
│       └── platform/          # Implementações concretas acopladas a SO/Driver
│           ├── opengl/        # OpenGLRendererAPI, OpenGLBuffer, OpenGLShader, OpenGLTexture
│           ├── glfw/          # GlfwWindow (ou SDLWindow)
│           └── windows/       # Hooks nativos Win32 (se aplicável)
├── sandbox/                   # Executável de validação de gameplay e estresse de API
│   ├── CMakeLists.txt
│   └── src/
│       └── main.cpp           # Especialização de Application para teste de mecânicas
├── editor/                    # Ferramenta transitória de instrumentação / Protótipo
│   ├── CMakeLists.txt
│   └── src/
│       └── main.cpp           # Painéis de inspeção de estado e overlays de depuração
└── vendor/                    # Dependências isoladas (submódulos Git ou header-only)
    ├── glm/                   # Álgebra linear (vetores, matrizes)
    ├── glad/                  # Carregador de ponteiros de função OpenGL
    ├── glfw/                  # Gerenciamento de janela e contexto gráfico nativo
    ├── spdlog/                # Sistema assíncrono de logging
    └── imgui/                 # GUI imediata (utilizada pela camada transitória de debug)
```

---

## 2. Grafo de Dependências e Contratos de Isolamento

O direcionamento das dependências é rigorosamente acíclico e unidirecional:

```text
[ vendor/ (glm, glad, glfw) ]
       ▲
       │ (privado ou exposto via wrappers controlados)
       │
[ engine/ (Core + Abstrações) ]
       ▲                      ▲
       │                      │
[ sandbox/ ]             [ editor/ (Transitório) ]
```

### Regras de Ouro de Isolamento
1. **Inversão de Dependência:** A pasta `engine/` não inclui nenhum cabeçalho de `sandbox/` ou `editor/`. O motor expõe interfaces abstratas (`Application`, `Layer`, `Window`); os clientes concretos derivam dessas classes e injetam suas lógicas específicas no loop de execução.
2. **Confinamento de Plataforma e APIs Gráficas:**
   * Cabeçalhos em `engine/include/engine/renderer/` nunca devem incluir referências diretas a drivers, como `<glad/glad.h>` ou `<GLFW/glfw3.h>`.
   * Toda chamada de API de baixo nível (`glDrawElements`, `glBindBuffer`, manipulação de `HWND`) deve existir exclusivamente encapsulada dentro de arquivos localizados sob `engine/src/platform/`.
3. **Imunidade da Sandbox ao Editor:** O target executável `sandbox` não referencia código do `editor`. A validação das rotinas centrais de jogo deve rodar de maneira autônoma, sem supor a existência de docks, inspectores ou editores de propriedades.

---

## 3. Arquitetura dos Subsistemas da Engine

### 3.1 Camada de Núcleo (Core Engine)
* **Application (Singleton / Controlador de Ciclo de Vida):** Orquestra o ciclo mestre do processo. Mantém a propriedade da janela principal (`Window`), gerencia a pilha de camadas (`LayerStack`) e executa o Game Loop contínuo até que uma flag interna de terminação seja acionada.
* **Window Abstraction:** Uma interface virtual pura (`Window.hpp`) fornecendo operações para coleta de eventos, sincronização vertical (VSync), redimensionamento e swap de buffers gráficos. A implementação concreta em `engine/src/platform/glfw/` herda essa interface, encapsulando os ponteiros e callbacks nativos da biblioteca de janelas.
* **Timestep (Delta Time):** Estrutura imutável passada a cada quadro para atualizar a lógica de física e gameplay, normalizando velocidades e variações de taxa de quadros (FPS):
  $$\Delta t = t_{\text{atual}} - t_{\text{anterior}}$$
* **Logger:** Camada de logging estruturado (via `spdlog` ou implementação customizada) segregada em dois canais globais distintos:
  * `CORE_LOG_*`: Mensagens de inicialização, alocação de buffers, compilação de shaders e falhas de subsistemas nativos.
  * `APP_LOG_*`: Mensagens oriundas exclusivamente da lógica do cliente (`sandbox`).

### 3.2 Sistema de Eventos (Event System)
A engine adota um modelo híbrido com foco em despacho síncrono imediato para eventos de sistema e pooling desacoplado para input contínuo:
* **Eventos Bloqueantes/Disparados:** Janela fechada, redimensionamento de buffer de tela, foco de janela e cliques únicos de teclado/mouse. A janela nativa captura os sinais do sistema operacional e delega para uma função de callback registrada pela `Application`.
* **Event Dispatcher:** Uma classe auxiliar percorre dinamicamente a pilha de camadas ativas (`LayerStack`) em ordem reversa (de cima para baixo), despachando o evento via templates baseados no tipo estático até que uma camada consuma o evento (`event.handled = true`).

```text
[ SO / Window Callbacks ]
          │
          ▼
   [ Application ]
          │ (despacha evento bruto)
          ▼
   [ EventDispatcher ]
          │
          ├─► [ Layer N (Editor / Debug Overlay) ] ── (Consumido? Para aqui)
          │
          └─► [ Layer 0 (Sandbox Game Logic) ]
```

### 3.3 Pipeline de Renderização (Renderer Architecture)
Para impedir acoplamentos prematuros com drivers específicos e habilitar batching eficiente, a renderização divide-se em três camadas:

1. **Camada de Comando e Abstração de Hardware:**
   * `RendererAPI`: Classe base abstrata contendo primitivas atômicas (`setClearColor`, `clear`, `drawIndexed`).
   * `RenderCommand`: Fachada estática com métodos inline que encaminha comandos de baixo nível para a instância ativa de `RendererAPI`.
   * `VertexArray`, `VertexBuffer`, `IndexBuffer`, `Shader`, `Texture`: Classes puramente virtuais cujas implementações finais instanciam os objetos correspondentes de GPU (ex: `OpenGLVertexArray`, `OpenGLBuffer`).
2. **Camada de Renderização de Alto Nível (Renderer2D):**
   * Não expõe chamadas pontuais de OpenGL ao cliente. Expõe operações semânticas do tipo `drawQuad(position, size, color)` e `drawQuad(position, size, texture)`.
   * **Batching Renderer:** Agrupa centenas de primitivas retangulares em um único `VertexBuffer` dinâmico por frame, minimizando *draw calls* e trocas de contexto na GPU. A submissão só é enviada ao pipeline gráfico quando o buffer satura ou ocorre uma alteração no estado de renderização (ex: troca de shader ou saturação de slots de textura).

---

## 4. Evolução da Camada de Ferramentas (Editor Roadmap)

O ciclo de vida das ferramentas divide-se em duas fases fundamentais para não atrasar a entrega da biblioteca base:

```text
[ FASE 1: v1.0 Core Engine ]
┌──────────────────────────────────────────────┐
│ sandbox (Executável Único)                   │
│  ├─ Gameplay Layer                           │
│  └─ Debug ImGui Layer (Injetada no Sandbox)  │
└──────────────────────────────────────────────┘
  * Editor como projeto independente é opcional ou protótipo isolado.

[ FASE 2: Engine Madura ]
┌──────────────────────────────────────────────┐
│ editor (Executável Independente)             │
│  ├─ Scene Hierarchy, Viewport Framebuffer    │
│  ├─ Asset Pipeline, Serializador de Cenas    │
│  └─ Gera / Empacota o build final            │
└──────────────────────────────────────────────┘
                       │
                       ▼ (Exporta artefatos e cenas)
┌──────────────────────────────────────────────┐
│ sandbox / Runtime Player (Executável Limpo)  │
│  └─ Consome engine pura + dados serializados │
└──────────────────────────────────────────────┘
```

### 4.1 Fase 1: O Editor Transitório (Core v1.0)
* **Objetivo:** Instrumentar o motor com métricas em tempo real sem complexidade de serialização de assets.
* **Mecanismo de Injeção:** A interface gráfica imediata (Dear ImGui) reside como uma `Layer` de diagnóstico. Essa camada pode ser compilada dentro do target `editor` provisório ou injetada diretamente no target `sandbox` por meio de flags de compilação condicionais (`#ifdef ENGINE_DEBUG`).
* **Responsabilidades Limitadas:**
  * Monitoramento de métricas do sistema: frame rate, tempo por quadro, consumo de memória da heap.
  * Estatísticas do pipeline gráfico: contagem total de *draw calls*, contagem de vértices/quads por frame.
  * Ajuste de variáveis em tempo real: alteração de cores, coeficientes de câmera e inspeção de transformações de teste.

### 4.2 Fase 2: O Editor como Produto Separado (Pós-Core)
* **Reescrita Estrutural:** O `editor` torna-se um ambiente de desenvolvimento completo e desacoplado.
* **Isolamento via Framebuffers:** O jogo não renderiza diretamente na janela do sistema operacional. O subsistema de renderização da engine desenha as entidades em uma textura atrelada a um *Framebuffer Object (FBO)* customizado. O editor consome essa textura e a renderiza dentro de um painel de Viewport flutuante.
* **Geração de Runtime:** O `editor` torna-se responsável pela edição, manipulação visual de componentes e serialização de grafos de cena (formatos JSON, YAML ou binário proprietário). O target `sandbox` é transformado em um executável limpo de runtime (o player final do jogo), cuja única função é ler os arquivos serializados pela ferramenta e rodar a engine sem overhead de debug.

---

## 5. Ciclo de Execução Principal (Game Loop Contract)

O fluxo síncrono executado a cada iteração do loop na classe `Application` respeita rigidamente a seguinte ordem:

1. **Processamento de Plataforma e Input:**
   * A janela nativa processa a fila de mensagens do sistema operacional (`Window::pollEvents()`).
   * Eventos disparados acionam callbacks e são propagados pela pilha de camadas ativas.
2. **Atualização Lógica (Update Phase):**
   * O tempo do quadro ($\Delta t$) é recalculado.
   * O loop itera progressivamente pela pilha de camadas, invocando `layer->onUpdate(ts)`.
   * Subsistemas lógicos, movimentação de entidades e algoritmos de cena calculam suas transformações.
3. **Fase de Renderização (Render Phase):**
   * Chamadas semânticas do renderizador submetem primitivas para a fila de comandos em memória.
   * `RenderCommand::clear()` prepara o buffer de cores e profundidade.
   * `Renderer2D::endScene()` descarrega (*flushes*) os lotes de vértices para a GPU via chamadas de desenho indexadas.
4. **Fase de Interface de Diagnóstico (Debug GUI Phase):**
   * Se o contexto de debug/ImGui estiver ativado, o pipeline abre o frame de interface imediata.
   * O loop itera pela pilha de camadas invocando `layer->onImGuiRender()`.
   * A geometria da interface gráfica é consolidada e despachada para a GPU sobreposta à cena.
5. **Apresentação de Vídeo:**
   * A janela executa a troca de buffers front e back (`Window::swapBuffers()`).
