# Shining Saturn 360 — v0.41b (snapshot de desenvolvimento)

**Tipo:** snapshot experimental de desenvolvimento; não é uma versão estável nem uma versão comprovada para executar jogos.

## Conteúdo incluído
- Núcleo SH-2 experimental com subconjunto de instruções e testes host.
- Biblioteca de jogos com ordenação/filtros e persistência experimental de favoritos.
- Scanner experimental de ROMs, associação de capas e atualização de metadados sem duplicar entradas.
- Módulo experimental de configurações versionadas com validação de valores.
- Workflow de auditoria/build Xenon e empacotamento XEX, quando a execução de CI conclui com sucesso.

## Limitações conhecidas
- Inicialização do `default.xex` em hardware Xbox 360 não confirmada nesta snapshot.
- Scanner e configurações ainda não estão ligados ao adaptador de armazenamento real do console.
- A interface visual completa, o mapeamento funcional de controles e os subsistemas VDP1, VDP2, SCSP, CD-ROM e SMPC não estão integrados como emulação funcional.
- Save Data, Save State e Load State ainda não estão implementados.
- Compatibilidade com jogos de Saturn não validada.

## Como obter o snapshot
1. Código-fonte: https://github.com/MeiKinosaki/Shining-Saturn-360/archive/refs/heads/main.zip
2. Artefatos de compilação: https://github.com/MeiKinosaki/Shining-Saturn-360/actions
3. Baixe o artefato `default.xex` ou ZIP apenas de uma execução concluída com sucesso. Artefatos do Actions são temporários e podem expirar.

## Critério de validação
Uma execução bem-sucedida do CI confirma somente os passos que o workflow realmente executou (por exemplo, compilação/testes host/empacotamento). Ela não confirma por si só que o XEX inicia em hardware real ou que jogos são executados.

**Versão do projeto mantida em v0.41b** até que um marco técnico maior seja validado.
