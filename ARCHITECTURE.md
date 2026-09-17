# Documento de Arquitetura Conceitual da HellixEngine (ARCHITECTURE.md)

Este documento especifica a arquitetura técnica, as regras de negócio, a taxonomia de tipos, o pipeline de dados e os padrões de organização de diretórios da **HellixEngine**. A engine adota o paradigma estrito de Programação Orientada a Objetos (POO Clássica) estruturado em **Scene Graph** (Grafos de Cena) e Composição de Componentes, rejeitando deliberadamente o paradigma de Entity Component System (ECS) baseado em tabelas lineares contínuas.

---

## 1. Visão Geral e Filosofia Arquitetural

A HellixEngine é construída para balancear didática técnica de baixo nível em C++20 com modularidade extensível e desacoplamento de responsabilidades. As diretrizes centrais que orientam a engenharia do projeto são:

1. **Abstração Baseada em Contratos e RAII:** Todo recurso do sistema operacional e de baixo nível (janela, contexto gráfico, buffers, shaders, texturas, arquivos e áudio) deve ser gerenciado estritamente sob o idioma de RAII (*Resource Acquisition Is Initialization*), garantindo ausência de vazamento de memória sem gerenciamento manual externo.


2. **Encapsulamento de APIs Nativas:** O subsistema cliente (Sandbox) e as camadas lógicas de jogo nunca realizam chamadas diretas a ponteiros brutos de GLFW, GLAD ou funções globais de OpenGL (`glDrawArrays`, `glBindTexture`, etc.). Toda operação gráfica é intermediada por fachadas tipadas e seguras.


3. **Hierarquia Limpa Orientada a Objetos:** Entidades são objetos polimórficos de primeira classe instanciados na heap com identificadores e matrizes espaciais próprias. A lógica de agregação ocorre via composição de componentes vinculados à entidade pai.


4. **Isolamento de Estado e Ciclo de Vida Determinístico:** A execução de cada quadro segue etapas imutáveis, garantindo que alterações físicas e lógicas estejam completamente calculadas antes da submissão para desenho.



---

## 2. Estrutura de Diretórios Recomendada

A árvore de pastas física reflete a modularidade lógica da engine e a segregação entre interfaces públicas (`include/`), implementações privadas (`src/`), ativos do jogo (`assets/`) e executáveis clientes (`sandbox/` e `editor/`):

```text
HellixEngine/
├── assets/
│   ├── audio/
│   │   ├── music/
│   │   └── sfx/
│   ├── fonts/
│   ├── scenes/
│   │   └── default_scene.json
│   ├── shaders/
│   │   ├── batch_quad.glsl
│   │   ├── standard_2d.vert
│   │   └── standard_2d.frag
│   └── textures/
│       └── sprites/
├── docs/
│   ├── ARCHITECTURE.md
│   ├── CONTRIBUTING.md
│   └── GIT_GUIDE.md
├── editor/
│   ├── include/
│   │   └── editor/
│   │       ├── panels/
│   │       │   ├── hierarchy_panel.hpp
│   │       │   └── inspector_panel.hpp
│   │       └── editor_layer.hpp
│   └── src/
│       ├── panels/
│       │   ├── hierarchy_panel.cpp
│       │   └── inspector_panel.cpp
│       ├── editor_layer.cpp
│       └── main.cpp
├── engine/
│   ├── include/
│   │   └── engine/
│   │       ├── core/
│   │       │   ├── application.hpp
│   │       │   ├── assert.hpp
│   │       │   ├── layer.hpp
│   │       │   ├── layer_stack.hpp
│   │       │   ├── log.hpp
│   │       │   ├── timestep.hpp
│   │       │   └── window.hpp
│   │       ├── events/
│   │       │   ├── application_event.hpp
│   │       │   ├── event.hpp
│   │       │   ├── key_event.hpp
│   │       │   └── mouse_event.hpp
│   │       ├── renderer/
│   │       │   ├── buffer.hpp
│   │       │   ├── camera2d.hpp
│   │       │   ├── render_command.hpp
│   │       │   ├── renderer2d.hpp
│   │       │   ├── renderer_api.hpp
│   │       │   ├── shader.hpp
│   │       │   ├── texture.hpp
│   │       │   └── vertex_array.hpp
│   │       ├── scene/
│   │       │   ├── component.hpp
│   │       │   ├── components.hpp
│   │       │   ├── entity.hpp
│   │       │   ├── scene.hpp
│   │       │   └── scene_camera.hpp
│   │       └── engine.hpp
│   └── src/
│       ├── core/
│       │   ├── application.cpp
│       │   ├── layer.cpp
│       │   ├── layer_stack.cpp
│       │   └── log.cpp
│       ├── platform/
│       │   ├── glfw/
│       │   │   ├── glfw_input.cpp
│       │   │   └── glfw_window.cpp
│       │   └── opengl/
│       │       ├── opengl_buffer.cpp
│       │       ├── opengl_context.cpp
│       │       ├── opengl_renderer_api.cpp
│       │       ├── opengl_shader.cpp
│       │       ├── opengl_texture.cpp
│       │       └── opengl_vertex_array.cpp
│       ├── renderer/
│       │   ├── camera2d.cpp
│       │   ├── render_command.cpp
│       │   └── renderer2d.cpp
│       └── scene/
│           ├── component.cpp
│           ├── entity.cpp
│           └── scene.cpp
├── sandbox/
│   ├── include/
│   │   └── sandbox_layer.hpp
│   └── src/
│       ├── main.cpp
│       └── sandbox_layer.cpp
├── vendor/
│   ├── GLFW/
│   ├── IMGUIDOCKING/
│   ├── MINIAUDIO/
│   ├── NLOHMANN/
│   ├── OPENGL/
│   ├── PYBIND11/
│   └── STB/
├── .gitignore
├── CMakeLists.txt
└── README.md

```

---

## 3. Regras de Negócio do Modelo de Mundo (POO Clássico)

Diferente de arquiteturas baseadas em dados esparsos como ECS, o modelo da HellixEngine ancora-se no encapsulamento de estado e hierarquia relacional:

```text
┌────────────────────────────────────────────────────────┐
│                        Scene                           │
│  - m_RootEntities: std::vector<std::shared_ptr<Entity>>│
│  - m_ActiveCamera: Camera2D                            │
└───────────────────────────┬────────────────────────────┘
                            │ possui 1..*
                            ▼
┌────────────────────────────────────────────────────────┐
│                       Entity                           │
│  - m_UUID: uint64_t                                    │
│  - m_Transform: Transform (Pos, Rot, Scale)            │
│  - m_Parent: std::weak_ptr<Entity>                     │
│  - m_Children: std::vector<std::shared_ptr<Entity>>    │
│  - m_Components: std::vector<std::unique_ptr<Component>│
└───────────────────────────┬────────────────────────────┘
                            │ compõe 0..*
                            ▼
┌────────────────────────────────────────────────────────┐
│                     Component                          │
│  - m_Owner: Entity*                                    │
│  + onAttach() / onUpdate(ts) / onRender()              │
└────────────────────────────────────────────────────────┘

```

### 3.1 A Cena (`Scene`)

* **Propriedade Única de Vida:** A `Scene` é a dona exclusiva da memória das entidades no topo da hierarquia (`Root Entities`) via referências fortes (`std::shared_ptr<Entity>`).


* **Gestão de Espaço:** Mantém referências à câmera de renderização ativa (`Camera2D` ou `SceneCamera`) e parâmetros de iluminação ambiente global.


* **Fases Determinísticas:** A classe `Scene` só permite a execução sequencial de dois métodos macro a cada quadro:
* `onUpdate(Timestep ts)`: Itera recursivamente por todas as entidades ativas chamando a lógica de script, propagação de transformações e subsistema físico.


* `onRender()`: Configura a matriz de visualização-projeção da câmera ativa e ordena a submissão dos nós com componentes visuais ao `Renderer2D`.





### 3.2 A Entidade (`Entity`)

* **Identidade Imutável:** Cada entidade recebe na construção um identificador universal (`UUID`) persistente e uma string textual descritiva de debug.


* **Topologia de Nós (Scene Graph):**
* Toda entidade armazena um ponteiro fraco (`std::weak_ptr<Entity>`) para o seu pai, evitando referências circulares e vazamento de memória cíclica.


* Mantém uma lista de ponteiros compartilhados (`std::vector<std::shared_ptr<Entity>>`) para suas entidades filhas.


* Toda entidade possui nativamente uma estrutura de transformação espacial (`Transform`):
* Posição Local (`Vector2`), Rotação Local (`float` em graus ou radianos) e Escala Local (`Vector2`).
* Matriz de Transformação Global (`Matrix4x4`), computada a partir da multiplicação:

$$\mathbf{M}_{\text{global}} = \mathbf{M}_{\text{pai}} \times \mathbf{M}_{\text{local}}$$



Se a entidade não tiver pai, $\mathbf{M}_{\text{global}} = \mathbf{M}_{\text{local}}$.




* **Composição de Componentes:**
* Componentes são instanciados e armazenados via `std::unique_ptr<Component>`.


* Cada tipo derivado de `Component` deve existir no máximo uma única vez por entidade (política de tipo exclusivo), impedindo múltiplos `SpriteRenderer` no mesmo nó.
* A busca de componentes por nós vizinhos é feita de forma estaticamente tipada via chamadas de template polimórficas (ex: `entity->getComponent<SpriteComponent>()`).



### 3.3 Componentes Primários Padronizados

Todos os componentes herdam de uma classe base pura `Component`, que fornece acesso direto ao ponteiro bruto da entidade hospedeira (`Entity* m_Owner`) e os métodos virtuais de ciclo de vida:

| Componente | Atributos Principais | Responsabilidade |
| --- | --- | --- |
| **`TransformComponent`** | Posição (`x, y`), Rotação (`z`), Escala (`sx, sy`). | Espacialização da entidade no mundo 2D.

|
| **`SpriteRendererComponent`** | Cor de Tintagem (`Vector4`), Textura (`shared_ptr<Texture2D>`), Tiling Factor (`float`). | Dados visuais submetidos ao pipeline de lote do renderer.

|
| **`CameraComponent`** | Zoom (`float`), Projeção Ortográfica, Flag de Câmera Primária (`bool`). | Define o ponto de visualização do mundo quando marcado como primário.

|
| **`ScriptComponent`** | Instância derivada de classe lógica de script nativo (`ScriptableEntity`). | Fornece hooks virtuais (`onCreate`, `onUpdate`, `onDestroy`) para lógica customizada do jogo.

|
| **`Collider2DComponent`** | Tipo de colisor (Caixa ou Círculo), Dimensões, Offset local, Flag de Trigger. | Suporte geométrico para detecção de colisão na fase de atualização.

|

---

## 4. Arquitetura da API Gráfica e Pipeline de Renderização

O subsistema gráfico é arquitetado em três níveis estritos de isolamento:

```text
┌────────────────────────────────────────────────────────┐
│                   Renderer2D (Alto Nível)              │
│  - drawQuad(position, size, color)                     │
│  - drawQuad(position, size, texture, tiling, tint)     │
│  - drawRotatedQuad(position, size, rotation, color)    │
└───────────────────────────┬────────────────────────────┘
                            │ submete vértices / índices
                            ▼
┌────────────────────────────────────────────────────────┐
│              RenderCommand (Fachada Estática)          │
│  - setViewport(x, y, w, h)                             │
│  - setClearColor(r, g, b, a)                           │
│  - clear()                                             │
│  - drawIndexed(vertexArray, indexCount)                │
└───────────────────────────┬────────────────────────────┘
                            │ consome polimorficamente
                            ▼
┌────────────────────────────────────────────────────────┐
│               RendererAPI (Interface Pura)             │
│  ▲                                                     │
│  └─────────── OpenGLRendererAPI (Implementação)        │
└────────────────────────────────────────────────────────┘

```

### 4.1 Regras de Negócio do Batch Renderer 2D

1. **Zero Draw Calls Individuais:** É expressamente proibido enviar dados de vértices à GPU isoladamente por sprite ou retângulo. O `Renderer2D` deve acumular vértices em um buffer dinâmico contínuo na memória da CPU (`Dynamic VBO`) e despachar um único comando de desenho (`glDrawElements`) ao atingir a capacidade do lote ou na finalização do quadro.


2. **Capacidade Máxima de Lote (Batch Limits):**
* Vértices por Quad: $4$
* Índices por Quad: $6$ (compostos por 2 triângulos: $0, 1, 2$ e $2, 3, 0$)
* Limite de Quads por Lote: $10.000$ quads
* Total de Vértices alocados estaticamente na VRAM: $40.000$ vértices
* Total de Índices pré-gerados na inicialização: $60.000$ índices


3. **Gerenciamento de Slots de Textura:**
* O pipeline deve interrogar a GPU no momento da inicialização (`glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS)`) para saber o limite de slots simultâneos suportados pelo hardware (geralmente entre 16 e 32 slots).
* O slot `0` é permanentemente reservado para uma textura 1x1 branca pura (usada para renderizar quads com cores sólidas sem alterar o shader).
* Se um quad submetido utilizar uma textura ainda não presente no lote atual, ela é alocada no próximo slot livre. Se todos os slots de textura forem esgotados, o `Renderer2D` deve forçar um flush automático (`endScene` imediato seguido de `beginScene`) para descarregar o lote atual e iniciar um novo.



### 4.2 Layout do Vértice Estruturado

Todo vértice trafegado pelo pipeline deve implementar rigidamente o seguinte empacotamento de bytes contíguos na memória:

```text
Offset 0x00:  Vector3  (Position)     -> 3 * float = 12 bytes
Offset 0x0C:  Vector4  (Color/Tint)   -> 4 * float = 16 bytes
Offset 0x1C:  Vector2  (TexCoords)    -> 2 * float =  8 bytes
Offset 0x24:  float    (TexIndex)     -> 1 * float =  4 bytes
Offset 0x28:  float    (TilingFactor) -> 1 * float =  4 bytes
Total por vértice: 44 bytes

```

### 4.3 Padrões de Arquivos de Shaders

A HellixEngine aceita duas convenções estritas para a pipeline de materiais e shaders na pasta `assets/shaders/`:

* **Padrão Dual (Recomendado para shaders simples):** Arquivos separados `.vert` e `.frag` com nomenclatura casada (ex.: `standard_2d.vert` e `standard_2d.frag`). Facilita realce de sintaxe em IDEs.
* **Padrão Unificado (Recomendado para shaders compostos):** Arquivo único com extensão `.glsl`. A engine lê o arquivo completo via streaming de texto e faz o parsing dos blocos através de pré-processamento interno baseado nas tags marcadoras:
* `#type vertex`
* `#type fragment`


* **Restrição de Versão do GLSL:** Todos os shaders devem declarar no topo `#version 330 core` para preservar compatibilidade integral entre Windows, Linux e macOS (quando via compatibilidade de perfis legados/MoltenGL).

---

## 5. Arquitetura do Sistema de Janela, Loop e Eventos

O gerenciamento de plataforma é segregado do loop central da aplicação através do desacoplamento por eventos direcionados.

```text
┌──────────────────┐
│   GLFW Callback  │
└────────┬─────────┘
         │ converte evento nativo do C
         ▼
┌──────────────────┐      despacha       ┌────────────────────────┐
│  Event (C++20)   ├────────────────────►│  Application          │
└──────────────────┘                     └──────────┬─────────────┘
                                                    │ propaga de trás p/ frente
                                                    ▼
                                         ┌────────────────────────┐
                                         │       LayerStack       │
                                         │  [ImGui Layer (Top)]   │
                                         │  [Gameplay Layer]      │
                                         │  [Background Layer]    │
                                         └────────────────────────┘

```

### 5.1 Regras de Negócio do Sistema de Eventos

1. **Eventos Baseados em Estruturas de Dados:** Todo evento gerado pelo sistema operacional (teclado, movimentação de cursor, clique, scroll, redimensionamento de viewport e fechamento de janela) é empacotado em uma classe concreta derivada de `Event`.


2. **Propagação Interrompível:** Cada instância de evento possui uma flag booleana `handled` inicializada como `false`. A `Application` itera a sua `LayerStack` do topo para a base (da camada mais externa/overlay para a mais profunda). Se uma camada marcar `event.handled = true` (ex: a janela do ImGui consumiu o clique do mouse), as camadas subsequentes ignoram o evento imediatamente.


3. **Despacho Estático via `EventDispatcher`:** O tratamento de eventos utiliza despacho tipado via funções lambda ou referências a métodos de membro (`std::bind` ou lambdas modernos), eliminando sequências manuais de `switch-case` com casting inseguro de ponteiros `void*`.



### 5.2 Estrutura da Camada e Pilha de Camadas (`Layer` e `LayerStack`)

* Uma `Layer` representa uma fatia independente de processamento e renderização (ex.: `GameLayer`, `EditorLayer`, `DebugOverlayLayer`).


* A `LayerStack` divide suas camadas em duas zonas:
* **Camadas Regulares:** Ficam na primeira metade da pilha e representam telas de menu, lógica de mundo e simulações ativas.


* **Overlays:** Camadas anexadas permanentemente no final da pilha. Recebem eventos prioritariamente e são sempre renderizadas por último (ex.: HUD de status, console de depuração, janelas flutuantes do ImGui).





---

## 6. O Ciclo de Execução de um Quadro (Frame Lifecycle)

O loop principal (`Application::run`) executa um fluxo de 7 passos síncronos e determinísticos:

```text
[Início do Frame]
       │
       ▼
 1. Cálculo de Delta Time (Timestep ts = tempoAtual - tempoUltimoFrame)
       │
       ▼
 2. Polling de Eventos do Sistema Operacional (glfwPollEvents)
       │  (Converte callbacks brutos em objetos da engine e despacha pela LayerStack)
       ▼
 3. Atualização Lógica das Camadas (Layer::onUpdate(ts))
       │  (Física, scripts da entidade, movimentação espacial e matrizes de cena)
       ▼
 4. Pré-Renderização e Limpeza de Buffers (RenderCommand::clear())
       │  (Limpeza de cor e buffer de profundidade se ativo)
       ▼
 5. Renderização da Cena e Flush Gráfico (Layer / Scene::onRender())
       │  (Submissão dos quads para o Renderer2D e disparo do draw indexed do lote)
       ▼
 6. Overlay de Depuração e UI (Layer::onImGuiRender())
       │  (Renderização das ferramentas do editor via Dear ImGui)
       ▼
 7. Apresentação do Quadro (Window::onUpdate() / glfwSwapBuffers)
       │
[Fim do Frame / Próxima Iteração]

```

---

## 7. Divisão de Responsabilidades e Limites dos 4 Subsistemas

Para viabilizar o fluxo de desenvolvimento paralelo entre os quatro integrantes do time, os limites de competência de cada subsistema são estabelecidos a seguir:

### Frente 1: Core & Platform Subsystem (Membro 1)

* **Responsabilidades:**
* Implementação da abstração de janela nativa encapsulada (`GlfwWindow`) com deleters customizados em ponteiros inteligentes.
* Estruturação do `Timestep` e contadores de framerate/delta time estáveis em alta precisão.
* Estrutura de dados e ciclo de vida da `LayerStack` e da classe base `Application`.


* Macros globais de asserção (`HELLIX_ASSERT`), profilaxia de memória e wrappers de logging contextual (`Log::CoreInfo`, `Log::CoreError`).


* **Fronteira Estrita:** Este módulo não pode depender de classes de nós de cena (`Entity`, `Component`) nem emitir instruções de submissão de desenho.



### Frente 2: Events & Input Subsystem (Membro 2)

* **Responsabilidades:**
* Modelagem de toda a taxonomia de classes de eventos (`WindowCloseEvent`, `WindowResizeEvent`, `KeyPressedEvent`, `MouseButtonPressedEvent`, etc.).


* Implementação do mecanismo `EventDispatcher`.


* Implementação do sistema de polling de entrada estático (`Input::isKeyPressed`, `Input::isMouseButtonPressed`, `Input::getMousePosition`).
* Conexão dos callbacks de baixo nível do GLFW com a emissão estruturada dos eventos da engine.


* **Fronteira Estrita:** O sistema de eventos apenas transita dados em memória; ele não processa física, não modifica posições de entidades e não decide o comportamento gráfico do jogo.



### Frente 3: Low-Level Graphics Subsystem (Membro 3)

* **Responsabilidades:**
* Abstração orientada a objetos de recursos de baixo nível da GPU: `VertexBuffer`, `IndexBuffer`, `VertexArray` e `Shader`.


* Implementação dos contratos virtuais da `RendererAPI` e especialização em `OpenGLRendererAPI`.


* Classe estática intermediária `RenderCommand` para emissão desacoplada de comandos da GPU.


* Carregamento, parsing e compilação de código de shader (`.glsl`, `.vert`, `.frag`) com verificação de erros de compilação/linkagem.


* **Fronteira Estrita:** Este subsistema cuida da infraestrutura de hardware; ele não gerencia lotes de quads, não sabe o que é uma textura de sprite e não possui conhecimento sobre o grafo de cena.



### Frente 4: Renderer2D, Scene & World Subsystem (Membro 4)

* **Responsabilidades:**
* Implementação da máquina de estados do `Renderer2D` com suporte completo a Batch Rendering.


* Estrutura do Grafo de Cena: classes `Scene`, `Entity` e o contrato base `Component`.


* Componentes essenciais de mundo: `TransformComponent`, `SpriteRendererComponent` e `CameraComponent`.


* Abstração de texturas bidimensionais (`Texture2D`) integradas à biblioteca `stb_image` e carregamento de arquivos de cena serializados.




* **Fronteira Estrita:** O subsistema de mundo delega todo desenho ao `Renderer2D`, que por sua vez delega toda comunicação com o hardware ao `RenderCommand`. O mundo não deve chamar funções OpenGL puras.



---

## 8. Convenções de Nomenclatura e Estilo de Código

* **Namespaces:** Toda a infraestrutura da engine reside no namespace raiz `hellix` e em seus sub-namespaces de módulo: `hellix::core`, `hellix::events`, `hellix::renderer`, `hellix::scene`.
* **Arquivos:** Nomes de arquivos em `snake_case` com extensões explícitas:
* Headers C++: `.hpp`
* Implementações C++: `.cpp`
* Shaders: `.glsl`, `.vert`, `.frag`


* **Membros Privados:** Devem utilizar obrigatoriamente o prefixo `m_` (exemplo: `m_window`, `m_running`, `m_transform`).
* **Membros Estáticos:** Devem utilizar obrigatoriamente o prefixo `s_` (exemplo: `s_instance`, `s_rendererAPI`).
* **Ponteiros Inteligentes:**
* Posse exclusiva: sempre `std::unique_ptr<T>`.
* Recursos compartilhados (texturas, shaders, malhas): `std::shared_ptr<T>`.
* Referências fracas (entidades pai, referências observadoras): `std::weak_ptr<T>`.
* Ponteiros brutos (`T*`) são permitidos estritamente como referências de observação não-proprietárias e temporárias (ex.: `Component::getOwner()`).