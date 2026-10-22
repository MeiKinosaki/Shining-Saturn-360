# Desenvolvimento modular e integração contínua — v0.47b

## Objetivo e regra de versão

Este documento organiza o trabalho do Shining Saturn 360 em cinco frentes com contratos explícitos. A versão permanece v0.47b durante a integração destas práticas. Só promover para v0.48b quando um marco verificável estiver concluído e documentado.

Não confundir código escrito com código compilado; testes de host com teste no Xbox 360 físico; geração de ELF/XEX com inicialização funcional no console; componentes simplificados com emulação fiel do Sega Saturn.

## Frentes e limites

### A — Núcleo SH-2
Responsável por registradores, instruções, flags, fluxo de controle/delay slots, exceções, interrupções e integração com o barramento. Cada instrução nova deve ter teste positivo, casos de limites/flags quando aplicável e regressão da sequência de execução. Snapshots de CPU não são save states completos.

### B — Hardware Saturn
Responsável por mapa de memória e MMIO, VDP1, VDP2, SCU/DMA, temporização, CD Block, SCSP e SMPC/controles. Os módulos devem receber dependências por interfaces explícitas e evitar incluir detalhes de UI ou de sistema de arquivos. Testes unitários devem verificar limites, alinhamento, conclusão de DMA, eventos e resultados por frame.

### C — Plataforma Xbox 360
Responsável por libXenon, entrada de controle, filesystem, rede, áudio/vídeo de plataforma, ELF32 e empacotamento XEX. Código específico de plataforma deve ficar atrás de adaptadores para que os testes de host possam usar implementações falsas. Wi-Fi e Ethernet usam o serviço de rede configurado no console; não se deve presumir suporte HTTP até o adaptador real ser implementado e testado.

### D — Interface e persistência
Responsável por biblioteca, scanner de ROMs, capas/cache, favoritos, filtros, configurações, menu, música de abertura e save/load state. A UI não deve executar diretamente lógica de CPU ou hardware. Save state completo deve ser versionado e conter estado consistente de CPU, RAM e dispositivos; arquivos inválidos devem ser rejeitados sem alterar o estado ativo.

### E — Integração e validação
Responsável por compilar testes de host, executar regressões, produzir relatórios, medir desempenho e verificar artefatos. Nenhum marco pode ser promovido se os testes relacionados falharem ou se o relatório não identificar claramente o que foi e não foi validado.

## Contratos de integração

- CPU lê/escreve através de interfaces de barramento, sem depender da implementação de UI.
- Hardware expõe registradores/eventos por contratos explícitos e mantém temporização determinística nos testes.
- Plataforma fornece adaptadores de filesystem, rede, áudio, vídeo e controles.
- Biblioteca e save states dependem de interfaces de armazenamento; não fazem chamadas de plataforma diretamente.
- A UI apenas apresenta estados e envia comandos ao núcleo/sistema.
- Artefatos devem registrar commit de origem, toolchain, comando de build e SHA-256.

## Integração contínua obrigatória

Para cada alteração relevante:
1. Compilar e executar os testes de host afetados.
2. Executar a suíte completa de regressão.
3. Quando o build de plataforma estiver integrado ao código atual, compilar ELF32 Xenon a partir do mesmo commit.
4. Empacotar default.xex apenas a partir do ELF aprovado e verificar sua estrutura.
5. Anexar logs, checksums e resumo de CI.
6. Não publicar uma nova versão se qualquer validação obrigatória falhar.

### Limitação importante do pipeline atual

O workflow existente compila o bootstrap Xenon extraído de arquivos ZIP legados que contêm seu próprio Makefile; isso não prova que todo o código em src/saturn/ está incluído nesse ELF/XEX. A suíte de host compila os fontes atuais, mas a ligação do núcleo Saturn completo ao executável de console ainda é um marco separado. O relatório de CI deve manter essa distinção explícita até a migração do build para os fontes atuais ser concluída.

## Perfilamento

Antes de otimizar, registrar uma baseline repetível. Medir separadamente:
- instruções SH-2 executadas por frame;
- tempo do núcleo SH-2;
- tempo de DMA;
- tempo de VDP1 e composição VDP2;
- custo de acessos MMIO;
- tempo total por frame e percentis, quando o relógio de plataforma permitir.

Comparar o mesmo teste, compilador, flags e workload. Não declarar ganho de desempenho com base apenas em contagens de instruções ou em estimativas.

## Reaproveitamento de código e licenças

Antes de portar código de outro emulador, registrar URL/revisão, licença, arquivos reutilizados, alterações e avisos exigidos. Preservar atribuições e avisos de copyright. Adaptar semântica e temporização para o hardware Saturn; semelhança de arquitetura não significa compatibilidade direta.

## Critérios de promoção para v0.48b

- Exceções/interrupções com testes reproduzíveis e integração com o fluxo SH-2/barramento;
- regressão de host aprovada no commit candidato;
- limites do bootstrap Xenon documentados e sem alegação de que ele já emula jogos;
- logs e checksums anexados aos artefatos disponíveis;
- documento de versão atualizado apenas depois das validações.
