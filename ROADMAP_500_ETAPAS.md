# Shining Saturn 360 — plano sequencial de 500 etapas

**Versão documentada mais recente:** v0.46b (incremento do SH-2 registrado; validação CI dos novos commits ainda deve ser confirmada).  
**Estado deste documento:** planejamento; as etapas não são declaradas concluídas por estarem listadas.  
**Regra de conclusão:** marcar uma etapa como concluída somente com alteração real no repositório ou teste/evidência correspondente. CI verde não prova execução em Xbox 360 físico nem compatibilidade com jogos comerciais.

## Como acompanhar
- As 500 etapas estão numeradas sequencialmente e divididas em cinco blocos de 100.
- Atualizar status e links de commits conforme cada etapa for implementada e validada.
- Para validar v0.46b, revisar testes SH-2, build ELF32, empacotamento `default.xex` e notas de versão. A promoção não prova boot em hardware real.
- Não declarar um subsistema completo enquanto o comportamento real do hardware e a integração não estiverem demonstrados.

## Bloco 1 — etapas 1–100: CPU SH-2, registradores, instruções e exceções
001. **decodificação de opcode** — documentar requisitos e referências técnicas.
002. **decodificação de opcode** — definir interfaces e invariantes do subsistema.
003. **decodificação de opcode** — implementar o caminho básico funcional.
004. **decodificação de opcode** — tratar limites, estados inválidos e erros.
005. **decodificação de opcode** — cobrir operações normais com testes unitários.
006. **decodificação de opcode** — adicionar casos extremos e regressões.
007. **decodificação de opcode** — integrar com o barramento e o scheduler.
008. **decodificação de opcode** — medir custo e corrigir gargalos sem alterar a semântica.
009. **decodificação de opcode** — registrar diagnóstico, limitações e evidências.
010. **decodificação de opcode** — executar validação de aceitação antes de marcar concluído.
011. **aritmética inteira** — documentar requisitos e referências técnicas.
012. **aritmética inteira** — definir interfaces e invariantes do subsistema.
013. **aritmética inteira** — implementar o caminho básico funcional.
014. **aritmética inteira** — tratar limites, estados inválidos e erros.
015. **aritmética inteira** — cobrir operações normais com testes unitários.
016. **aritmética inteira** — adicionar casos extremos e regressões.
017. **aritmética inteira** — integrar com o barramento e o scheduler.
018. **aritmética inteira** — medir custo e corrigir gargalos sem alterar a semântica.
019. **aritmética inteira** — registrar diagnóstico, limitações e evidências.
020. **aritmética inteira** — executar validação de aceitação antes de marcar concluído.
021. **lógica e comparações** — documentar requisitos e referências técnicas.
022. **lógica e comparações** — definir interfaces e invariantes do subsistema.
023. **lógica e comparações** — implementar o caminho básico funcional.
024. **lógica e comparações** — tratar limites, estados inválidos e erros.
025. **lógica e comparações** — cobrir operações normais com testes unitários.
026. **lógica e comparações** — adicionar casos extremos e regressões.
027. **lógica e comparações** — integrar com o barramento e o scheduler.
028. **lógica e comparações** — medir custo e corrigir gargalos sem alterar a semântica.
029. **lógica e comparações** — registrar diagnóstico, limitações e evidências.
030. **lógica e comparações** — executar validação de aceitação antes de marcar concluído.
031. **deslocamentos e rotações** — documentar requisitos e referências técnicas.
032. **deslocamentos e rotações** — definir interfaces e invariantes do subsistema.
033. **deslocamentos e rotações** — implementar o caminho básico funcional.
034. **deslocamentos e rotações** — tratar limites, estados inválidos e erros.
035. **deslocamentos e rotações** — cobrir operações normais com testes unitários.
036. **deslocamentos e rotações** — adicionar casos extremos e regressões.
037. **deslocamentos e rotações** — integrar com o barramento e o scheduler.
038. **deslocamentos e rotações** — medir custo e corrigir gargalos sem alterar a semântica.
039. **deslocamentos e rotações** — registrar diagnóstico, limitações e evidências.
040. **deslocamentos e rotações** — executar validação de aceitação antes de marcar concluído.
041. **multiplicação e divisão** — documentar requisitos e referências técnicas.
042. **multiplicação e divisão** — definir interfaces e invariantes do subsistema.
043. **multiplicação e divisão** — implementar o caminho básico funcional.
044. **multiplicação e divisão** — tratar limites, estados inválidos e erros.
045. **multiplicação e divisão** — cobrir operações normais com testes unitários.
046. **multiplicação e divisão** — adicionar casos extremos e regressões.
047. **multiplicação e divisão** — integrar com o barramento e o scheduler.
048. **multiplicação e divisão** — medir custo e corrigir gargalos sem alterar a semântica.
049. **multiplicação e divisão** — registrar diagnóstico, limitações e evidências.
050. **multiplicação e divisão** — executar validação de aceitação antes de marcar concluído.
051. **acessos à memória** — documentar requisitos e referências técnicas.
052. **acessos à memória** — definir interfaces e invariantes do subsistema.
053. **acessos à memória** — implementar o caminho básico funcional.
054. **acessos à memória** — tratar limites, estados inválidos e erros.
055. **acessos à memória** — cobrir operações normais com testes unitários.
056. **acessos à memória** — adicionar casos extremos e regressões.
057. **acessos à memória** — integrar com o barramento e o scheduler.
058. **acessos à memória** — medir custo e corrigir gargalos sem alterar a semântica.
059. **acessos à memória** — registrar diagnóstico, limitações e evidências.
060. **acessos à memória** — executar validação de aceitação antes de marcar concluído.
061. **desvios e delay slots** — documentar requisitos e referências técnicas.
062. **desvios e delay slots** — definir interfaces e invariantes do subsistema.
063. **desvios e delay slots** — implementar o caminho básico funcional.
064. **desvios e delay slots** — tratar limites, estados inválidos e erros.
065. **desvios e delay slots** — cobrir operações normais com testes unitários.
066. **desvios e delay slots** — adicionar casos extremos e regressões.
067. **desvios e delay slots** — integrar com o barramento e o scheduler.
068. **desvios e delay slots** — medir custo e corrigir gargalos sem alterar a semântica.
069. **desvios e delay slots** — registrar diagnóstico, limitações e evidências.
070. **desvios e delay slots** — executar validação de aceitação antes de marcar concluído.
071. **SR/PR/GBR/VBR/MACH/MACL** — documentar requisitos e referências técnicas.
072. **SR/PR/GBR/VBR/MACH/MACL** — definir interfaces e invariantes do subsistema.
073. **SR/PR/GBR/VBR/MACH/MACL** — implementar o caminho básico funcional.
074. **SR/PR/GBR/VBR/MACH/MACL** — tratar limites, estados inválidos e erros.
075. **SR/PR/GBR/VBR/MACH/MACL** — cobrir operações normais com testes unitários.
076. **SR/PR/GBR/VBR/MACH/MACL** — adicionar casos extremos e regressões.
077. **SR/PR/GBR/VBR/MACH/MACL** — integrar com o barramento e o scheduler.
078. **SR/PR/GBR/VBR/MACH/MACL** — medir custo e corrigir gargalos sem alterar a semântica.
079. **SR/PR/GBR/VBR/MACH/MACL** — registrar diagnóstico, limitações e evidências.
080. **SR/PR/GBR/VBR/MACH/MACL** — executar validação de aceitação antes de marcar concluído.
081. **exceções e interrupções** — documentar requisitos e referências técnicas.
082. **exceções e interrupções** — definir interfaces e invariantes do subsistema.
083. **exceções e interrupções** — implementar o caminho básico funcional.
084. **exceções e interrupções** — tratar limites, estados inválidos e erros.
085. **exceções e interrupções** — cobrir operações normais com testes unitários.
086. **exceções e interrupções** — adicionar casos extremos e regressões.
087. **exceções e interrupções** — integrar com o barramento e o scheduler.
088. **exceções e interrupções** — medir custo e corrigir gargalos sem alterar a semântica.
089. **exceções e interrupções** — registrar diagnóstico, limitações e evidências.
090. **exceções e interrupções** — executar validação de aceitação antes de marcar concluído.
091. **temporização e regressões** — documentar requisitos e referências técnicas.
092. **temporização e regressões** — definir interfaces e invariantes do subsistema.
093. **temporização e regressões** — implementar o caminho básico funcional.
094. **temporização e regressões** — tratar limites, estados inválidos e erros.
095. **temporização e regressões** — cobrir operações normais com testes unitários.
096. **temporização e regressões** — adicionar casos extremos e regressões.
097. **temporização e regressões** — integrar com o barramento e o scheduler.
098. **temporização e regressões** — medir custo e corrigir gargalos sem alterar a semântica.
099. **temporização e regressões** — registrar diagnóstico, limitações e evidências.
100. **temporização e regressões** — executar validação de aceitação antes de marcar concluído.

## Bloco 2 — etapas 101–200: Memória Saturn, barramento, MMIO e SCU/DMA
101. **mapa de memória real** — documentar requisitos e referências técnicas.
102. **mapa de memória real** — definir interfaces e invariantes do subsistema.
103. **mapa de memória real** — implementar o caminho básico funcional.
104. **mapa de memória real** — tratar limites, estados inválidos e erros.
105. **mapa de memória real** — cobrir operações normais com testes unitários.
106. **mapa de memória real** — adicionar casos extremos e regressões.
107. **mapa de memória real** — integrar com o barramento e o scheduler.
108. **mapa de memória real** — medir custo e corrigir gargalos sem alterar a semântica.
109. **mapa de memória real** — registrar diagnóstico, limitações e evidências.
110. **mapa de memória real** — executar validação de aceitação antes de marcar concluído.
111. **work RAM baixa e alta** — documentar requisitos e referências técnicas.
112. **work RAM baixa e alta** — definir interfaces e invariantes do subsistema.
113. **work RAM baixa e alta** — implementar o caminho básico funcional.
114. **work RAM baixa e alta** — tratar limites, estados inválidos e erros.
115. **work RAM baixa e alta** — cobrir operações normais com testes unitários.
116. **work RAM baixa e alta** — adicionar casos extremos e regressões.
117. **work RAM baixa e alta** — integrar com o barramento e o scheduler.
118. **work RAM baixa e alta** — medir custo e corrigir gargalos sem alterar a semântica.
119. **work RAM baixa e alta** — registrar diagnóstico, limitações e evidências.
120. **work RAM baixa e alta** — executar validação de aceitação antes de marcar concluído.
121. **ROM/BIOS e aliases** — documentar requisitos e referências técnicas.
122. **ROM/BIOS e aliases** — definir interfaces e invariantes do subsistema.
123. **ROM/BIOS e aliases** — implementar o caminho básico funcional.
124. **ROM/BIOS e aliases** — tratar limites, estados inválidos e erros.
125. **ROM/BIOS e aliases** — cobrir operações normais com testes unitários.
126. **ROM/BIOS e aliases** — adicionar casos extremos e regressões.
127. **ROM/BIOS e aliases** — integrar com o barramento e o scheduler.
128. **ROM/BIOS e aliases** — medir custo e corrigir gargalos sem alterar a semântica.
129. **ROM/BIOS e aliases** — registrar diagnóstico, limitações e evidências.
130. **ROM/BIOS e aliases** — executar validação de aceitação antes de marcar concluído.
131. **endianness e alinhamento** — documentar requisitos e referências técnicas.
132. **endianness e alinhamento** — definir interfaces e invariantes do subsistema.
133. **endianness e alinhamento** — implementar o caminho básico funcional.
134. **endianness e alinhamento** — tratar limites, estados inválidos e erros.
135. **endianness e alinhamento** — cobrir operações normais com testes unitários.
136. **endianness e alinhamento** — adicionar casos extremos e regressões.
137. **endianness e alinhamento** — integrar com o barramento e o scheduler.
138. **endianness e alinhamento** — medir custo e corrigir gargalos sem alterar a semântica.
139. **endianness e alinhamento** — registrar diagnóstico, limitações e evidências.
140. **endianness e alinhamento** — executar validação de aceitação antes de marcar concluído.
141. **decodificador de endereços** — documentar requisitos e referências técnicas.
142. **decodificador de endereços** — definir interfaces e invariantes do subsistema.
143. **decodificador de endereços** — implementar o caminho básico funcional.
144. **decodificador de endereços** — tratar limites, estados inválidos e erros.
145. **decodificador de endereços** — cobrir operações normais com testes unitários.
146. **decodificador de endereços** — adicionar casos extremos e regressões.
147. **decodificador de endereços** — integrar com o barramento e o scheduler.
148. **decodificador de endereços** — medir custo e corrigir gargalos sem alterar a semântica.
149. **decodificador de endereços** — registrar diagnóstico, limitações e evidências.
150. **decodificador de endereços** — executar validação de aceitação antes de marcar concluído.
151. **registradores MMIO por dispositivo** — documentar requisitos e referências técnicas.
152. **registradores MMIO por dispositivo** — definir interfaces e invariantes do subsistema.
153. **registradores MMIO por dispositivo** — implementar o caminho básico funcional.
154. **registradores MMIO por dispositivo** — tratar limites, estados inválidos e erros.
155. **registradores MMIO por dispositivo** — cobrir operações normais com testes unitários.
156. **registradores MMIO por dispositivo** — adicionar casos extremos e regressões.
157. **registradores MMIO por dispositivo** — integrar com o barramento e o scheduler.
158. **registradores MMIO por dispositivo** — medir custo e corrigir gargalos sem alterar a semântica.
159. **registradores MMIO por dispositivo** — registrar diagnóstico, limitações e evidências.
160. **registradores MMIO por dispositivo** — executar validação de aceitação antes de marcar concluído.
161. **arbitragem e contenção** — documentar requisitos e referências técnicas.
162. **arbitragem e contenção** — definir interfaces e invariantes do subsistema.
163. **arbitragem e contenção** — implementar o caminho básico funcional.
164. **arbitragem e contenção** — tratar limites, estados inválidos e erros.
165. **arbitragem e contenção** — cobrir operações normais com testes unitários.
166. **arbitragem e contenção** — adicionar casos extremos e regressões.
167. **arbitragem e contenção** — integrar com o barramento e o scheduler.
168. **arbitragem e contenção** — medir custo e corrigir gargalos sem alterar a semântica.
169. **arbitragem e contenção** — registrar diagnóstico, limitações e evidências.
170. **arbitragem e contenção** — executar validação de aceitação antes de marcar concluído.
171. **SCU e interrupções** — documentar requisitos e referências técnicas.
172. **SCU e interrupções** — definir interfaces e invariantes do subsistema.
173. **SCU e interrupções** — implementar o caminho básico funcional.
174. **SCU e interrupções** — tratar limites, estados inválidos e erros.
175. **SCU e interrupções** — cobrir operações normais com testes unitários.
176. **SCU e interrupções** — adicionar casos extremos e regressões.
177. **SCU e interrupções** — integrar com o barramento e o scheduler.
178. **SCU e interrupções** — medir custo e corrigir gargalos sem alterar a semântica.
179. **SCU e interrupções** — registrar diagnóstico, limitações e evidências.
180. **SCU e interrupções** — executar validação de aceitação antes de marcar concluído.
181. **DMA origem/destino/tamanho** — documentar requisitos e referências técnicas.
182. **DMA origem/destino/tamanho** — definir interfaces e invariantes do subsistema.
183. **DMA origem/destino/tamanho** — implementar o caminho básico funcional.
184. **DMA origem/destino/tamanho** — tratar limites, estados inválidos e erros.
185. **DMA origem/destino/tamanho** — cobrir operações normais com testes unitários.
186. **DMA origem/destino/tamanho** — adicionar casos extremos e regressões.
187. **DMA origem/destino/tamanho** — integrar com o barramento e o scheduler.
188. **DMA origem/destino/tamanho** — medir custo e corrigir gargalos sem alterar a semântica.
189. **DMA origem/destino/tamanho** — registrar diagnóstico, limitações e evidências.
190. **DMA origem/destino/tamanho** — executar validação de aceitação antes de marcar concluído.
191. **testes e integração de barramento** — documentar requisitos e referências técnicas.
192. **testes e integração de barramento** — definir interfaces e invariantes do subsistema.
193. **testes e integração de barramento** — implementar o caminho básico funcional.
194. **testes e integração de barramento** — tratar limites, estados inválidos e erros.
195. **testes e integração de barramento** — cobrir operações normais com testes unitários.
196. **testes e integração de barramento** — adicionar casos extremos e regressões.
197. **testes e integração de barramento** — integrar com o barramento e o scheduler.
198. **testes e integração de barramento** — medir custo e corrigir gargalos sem alterar a semântica.
199. **testes e integração de barramento** — registrar diagnóstico, limitações e evidências.
200. **testes e integração de barramento** — executar validação de aceitação antes de marcar concluído.

## Bloco 3 — etapas 201–300: VDP1, VDP2, composição gráfica e sincronização
201. **comandos e listas VDP1** — documentar requisitos e referências técnicas.
202. **comandos e listas VDP1** — definir interfaces e invariantes do subsistema.
203. **comandos e listas VDP1** — implementar o caminho básico funcional.
204. **comandos e listas VDP1** — tratar limites, estados inválidos e erros.
205. **comandos e listas VDP1** — cobrir operações normais com testes unitários.
206. **comandos e listas VDP1** — adicionar casos extremos e regressões.
207. **comandos e listas VDP1** — integrar com o barramento e o scheduler.
208. **comandos e listas VDP1** — medir custo e corrigir gargalos sem alterar a semântica.
209. **comandos e listas VDP1** — registrar diagnóstico, limitações e evidências.
210. **comandos e listas VDP1** — executar validação de aceitação antes de marcar concluído.
211. **sprites e texturas** — documentar requisitos e referências técnicas.
212. **sprites e texturas** — definir interfaces e invariantes do subsistema.
213. **sprites e texturas** — implementar o caminho básico funcional.
214. **sprites e texturas** — tratar limites, estados inválidos e erros.
215. **sprites e texturas** — cobrir operações normais com testes unitários.
216. **sprites e texturas** — adicionar casos extremos e regressões.
217. **sprites e texturas** — integrar com o barramento e o scheduler.
218. **sprites e texturas** — medir custo e corrigir gargalos sem alterar a semântica.
219. **sprites e texturas** — registrar diagnóstico, limitações e evidências.
220. **sprites e texturas** — executar validação de aceitação antes de marcar concluído.
221. **polígonos e linhas** — documentar requisitos e referências técnicas.
222. **polígonos e linhas** — definir interfaces e invariantes do subsistema.
223. **polígonos e linhas** — implementar o caminho básico funcional.
224. **polígonos e linhas** — tratar limites, estados inválidos e erros.
225. **polígonos e linhas** — cobrir operações normais com testes unitários.
226. **polígonos e linhas** — adicionar casos extremos e regressões.
227. **polígonos e linhas** — integrar com o barramento e o scheduler.
228. **polígonos e linhas** — medir custo e corrigir gargalos sem alterar a semântica.
229. **polígonos e linhas** — registrar diagnóstico, limitações e evidências.
230. **polígonos e linhas** — executar validação de aceitação antes de marcar concluído.
231. **clipping e coordenadas** — documentar requisitos e referências técnicas.
232. **clipping e coordenadas** — definir interfaces e invariantes do subsistema.
233. **clipping e coordenadas** — implementar o caminho básico funcional.
234. **clipping e coordenadas** — tratar limites, estados inválidos e erros.
235. **clipping e coordenadas** — cobrir operações normais com testes unitários.
236. **clipping e coordenadas** — adicionar casos extremos e regressões.
237. **clipping e coordenadas** — integrar com o barramento e o scheduler.
238. **clipping e coordenadas** — medir custo e corrigir gargalos sem alterar a semântica.
239. **clipping e coordenadas** — registrar diagnóstico, limitações e evidências.
240. **clipping e coordenadas** — executar validação de aceitação antes de marcar concluído.
241. **framebuffer VDP1** — documentar requisitos e referências técnicas.
242. **framebuffer VDP1** — definir interfaces e invariantes do subsistema.
243. **framebuffer VDP1** — implementar o caminho básico funcional.
244. **framebuffer VDP1** — tratar limites, estados inválidos e erros.
245. **framebuffer VDP1** — cobrir operações normais com testes unitários.
246. **framebuffer VDP1** — adicionar casos extremos e regressões.
247. **framebuffer VDP1** — integrar com o barramento e o scheduler.
248. **framebuffer VDP1** — medir custo e corrigir gargalos sem alterar a semântica.
249. **framebuffer VDP1** — registrar diagnóstico, limitações e evidências.
250. **framebuffer VDP1** — executar validação de aceitação antes de marcar concluído.
251. **camadas VDP2** — documentar requisitos e referências técnicas.
252. **camadas VDP2** — definir interfaces e invariantes do subsistema.
253. **camadas VDP2** — implementar o caminho básico funcional.
254. **camadas VDP2** — tratar limites, estados inválidos e erros.
255. **camadas VDP2** — cobrir operações normais com testes unitários.
256. **camadas VDP2** — adicionar casos extremos e regressões.
257. **camadas VDP2** — integrar com o barramento e o scheduler.
258. **camadas VDP2** — medir custo e corrigir gargalos sem alterar a semântica.
259. **camadas VDP2** — registrar diagnóstico, limitações e evidências.
260. **camadas VDP2** — executar validação de aceitação antes de marcar concluído.
261. **scroll e planos** — documentar requisitos e referências técnicas.
262. **scroll e planos** — definir interfaces e invariantes do subsistema.
263. **scroll e planos** — implementar o caminho básico funcional.
264. **scroll e planos** — tratar limites, estados inválidos e erros.
265. **scroll e planos** — cobrir operações normais com testes unitários.
266. **scroll e planos** — adicionar casos extremos e regressões.
267. **scroll e planos** — integrar com o barramento e o scheduler.
268. **scroll e planos** — medir custo e corrigir gargalos sem alterar a semântica.
269. **scroll e planos** — registrar diagnóstico, limitações e evidências.
270. **scroll e planos** — executar validação de aceitação antes de marcar concluído.
271. **paletas e transparência** — documentar requisitos e referências técnicas.
272. **paletas e transparência** — definir interfaces e invariantes do subsistema.
273. **paletas e transparência** — implementar o caminho básico funcional.
274. **paletas e transparência** — tratar limites, estados inválidos e erros.
275. **paletas e transparência** — cobrir operações normais com testes unitários.
276. **paletas e transparência** — adicionar casos extremos e regressões.
277. **paletas e transparência** — integrar com o barramento e o scheduler.
278. **paletas e transparência** — medir custo e corrigir gargalos sem alterar a semântica.
279. **paletas e transparência** — registrar diagnóstico, limitações e evidências.
280. **paletas e transparência** — executar validação de aceitação antes de marcar concluído.
281. **prioridades e composição** — documentar requisitos e referências técnicas.
282. **prioridades e composição** — definir interfaces e invariantes do subsistema.
283. **prioridades e composição** — implementar o caminho básico funcional.
284. **prioridades e composição** — tratar limites, estados inválidos e erros.
285. **prioridades e composição** — cobrir operações normais com testes unitários.
286. **prioridades e composição** — adicionar casos extremos e regressões.
287. **prioridades e composição** — integrar com o barramento e o scheduler.
288. **prioridades e composição** — medir custo e corrigir gargalos sem alterar a semântica.
289. **prioridades e composição** — registrar diagnóstico, limitações e evidências.
290. **prioridades e composição** — executar validação de aceitação antes de marcar concluído.
291. **timing de quadro e testes gráficos** — documentar requisitos e referências técnicas.
292. **timing de quadro e testes gráficos** — definir interfaces e invariantes do subsistema.
293. **timing de quadro e testes gráficos** — implementar o caminho básico funcional.
294. **timing de quadro e testes gráficos** — tratar limites, estados inválidos e erros.
295. **timing de quadro e testes gráficos** — cobrir operações normais com testes unitários.
296. **timing de quadro e testes gráficos** — adicionar casos extremos e regressões.
297. **timing de quadro e testes gráficos** — integrar com o barramento e o scheduler.
298. **timing de quadro e testes gráficos** — medir custo e corrigir gargalos sem alterar a semântica.
299. **timing de quadro e testes gráficos** — registrar diagnóstico, limitações e evidências.
300. **timing de quadro e testes gráficos** — executar validação de aceitação antes de marcar concluído.

## Bloco 4 — etapas 301–400: CD Block, SCSP, SMPC e periféricos
301. **parser de imagem de disco** — documentar requisitos e referências técnicas.
302. **parser de imagem de disco** — definir interfaces e invariantes do subsistema.
303. **parser de imagem de disco** — implementar o caminho básico funcional.
304. **parser de imagem de disco** — tratar limites, estados inválidos e erros.
305. **parser de imagem de disco** — cobrir operações normais com testes unitários.
306. **parser de imagem de disco** — adicionar casos extremos e regressões.
307. **parser de imagem de disco** — integrar com o barramento e o scheduler.
308. **parser de imagem de disco** — medir custo e corrigir gargalos sem alterar a semântica.
309. **parser de imagem de disco** — registrar diagnóstico, limitações e evidências.
310. **parser de imagem de disco** — executar validação de aceitação antes de marcar concluído.
311. **TOC e faixas de CD** — documentar requisitos e referências técnicas.
312. **TOC e faixas de CD** — definir interfaces e invariantes do subsistema.
313. **TOC e faixas de CD** — implementar o caminho básico funcional.
314. **TOC e faixas de CD** — tratar limites, estados inválidos e erros.
315. **TOC e faixas de CD** — cobrir operações normais com testes unitários.
316. **TOC e faixas de CD** — adicionar casos extremos e regressões.
317. **TOC e faixas de CD** — integrar com o barramento e o scheduler.
318. **TOC e faixas de CD** — medir custo e corrigir gargalos sem alterar a semântica.
319. **TOC e faixas de CD** — registrar diagnóstico, limitações e evidências.
320. **TOC e faixas de CD** — executar validação de aceitação antes de marcar concluído.
321. **leitura de setores** — documentar requisitos e referências técnicas.
322. **leitura de setores** — definir interfaces e invariantes do subsistema.
323. **leitura de setores** — implementar o caminho básico funcional.
324. **leitura de setores** — tratar limites, estados inválidos e erros.
325. **leitura de setores** — cobrir operações normais com testes unitários.
326. **leitura de setores** — adicionar casos extremos e regressões.
327. **leitura de setores** — integrar com o barramento e o scheduler.
328. **leitura de setores** — medir custo e corrigir gargalos sem alterar a semântica.
329. **leitura de setores** — registrar diagnóstico, limitações e evidências.
330. **leitura de setores** — executar validação de aceitação antes de marcar concluído.
331. **comandos do CD Block** — documentar requisitos e referências técnicas.
332. **comandos do CD Block** — definir interfaces e invariantes do subsistema.
333. **comandos do CD Block** — implementar o caminho básico funcional.
334. **comandos do CD Block** — tratar limites, estados inválidos e erros.
335. **comandos do CD Block** — cobrir operações normais com testes unitários.
336. **comandos do CD Block** — adicionar casos extremos e regressões.
337. **comandos do CD Block** — integrar com o barramento e o scheduler.
338. **comandos do CD Block** — medir custo e corrigir gargalos sem alterar a semântica.
339. **comandos do CD Block** — registrar diagnóstico, limitações e evidências.
340. **comandos do CD Block** — executar validação de aceitação antes de marcar concluído.
341. **bootstrap BIOS e boot de mídia** — documentar requisitos e referências técnicas.
342. **bootstrap BIOS e boot de mídia** — definir interfaces e invariantes do subsistema.
343. **bootstrap BIOS e boot de mídia** — implementar o caminho básico funcional.
344. **bootstrap BIOS e boot de mídia** — tratar limites, estados inválidos e erros.
345. **bootstrap BIOS e boot de mídia** — cobrir operações normais com testes unitários.
346. **bootstrap BIOS e boot de mídia** — adicionar casos extremos e regressões.
347. **bootstrap BIOS e boot de mídia** — integrar com o barramento e o scheduler.
348. **bootstrap BIOS e boot de mídia** — medir custo e corrigir gargalos sem alterar a semântica.
349. **bootstrap BIOS e boot de mídia** — registrar diagnóstico, limitações e evidências.
350. **bootstrap BIOS e boot de mídia** — executar validação de aceitação antes de marcar concluído.
351. **canais SCSP** — documentar requisitos e referências técnicas.
352. **canais SCSP** — definir interfaces e invariantes do subsistema.
353. **canais SCSP** — implementar o caminho básico funcional.
354. **canais SCSP** — tratar limites, estados inválidos e erros.
355. **canais SCSP** — cobrir operações normais com testes unitários.
356. **canais SCSP** — adicionar casos extremos e regressões.
357. **canais SCSP** — integrar com o barramento e o scheduler.
358. **canais SCSP** — medir custo e corrigir gargalos sem alterar a semântica.
359. **canais SCSP** — registrar diagnóstico, limitações e evidências.
360. **canais SCSP** — executar validação de aceitação antes de marcar concluído.
361. **amostras e formatos de áudio** — documentar requisitos e referências técnicas.
362. **amostras e formatos de áudio** — definir interfaces e invariantes do subsistema.
363. **amostras e formatos de áudio** — implementar o caminho básico funcional.
364. **amostras e formatos de áudio** — tratar limites, estados inválidos e erros.
365. **amostras e formatos de áudio** — cobrir operações normais com testes unitários.
366. **amostras e formatos de áudio** — adicionar casos extremos e regressões.
367. **amostras e formatos de áudio** — integrar com o barramento e o scheduler.
368. **amostras e formatos de áudio** — medir custo e corrigir gargalos sem alterar a semântica.
369. **amostras e formatos de áudio** — registrar diagnóstico, limitações e evidências.
370. **amostras e formatos de áudio** — executar validação de aceitação antes de marcar concluído.
371. **mixagem e temporização** — documentar requisitos e referências técnicas.
372. **mixagem e temporização** — definir interfaces e invariantes do subsistema.
373. **mixagem e temporização** — implementar o caminho básico funcional.
374. **mixagem e temporização** — tratar limites, estados inválidos e erros.
375. **mixagem e temporização** — cobrir operações normais com testes unitários.
376. **mixagem e temporização** — adicionar casos extremos e regressões.
377. **mixagem e temporização** — integrar com o barramento e o scheduler.
378. **mixagem e temporização** — medir custo e corrigir gargalos sem alterar a semântica.
379. **mixagem e temporização** — registrar diagnóstico, limitações e evidências.
380. **mixagem e temporização** — executar validação de aceitação antes de marcar concluído.
381. **SMPC e controle Saturn** — documentar requisitos e referências técnicas.
382. **SMPC e controle Saturn** — definir interfaces e invariantes do subsistema.
383. **SMPC e controle Saturn** — implementar o caminho básico funcional.
384. **SMPC e controle Saturn** — tratar limites, estados inválidos e erros.
385. **SMPC e controle Saturn** — cobrir operações normais com testes unitários.
386. **SMPC e controle Saturn** — adicionar casos extremos e regressões.
387. **SMPC e controle Saturn** — integrar com o barramento e o scheduler.
388. **SMPC e controle Saturn** — medir custo e corrigir gargalos sem alterar a semântica.
389. **SMPC e controle Saturn** — registrar diagnóstico, limitações e evidências.
390. **SMPC e controle Saturn** — executar validação de aceitação antes de marcar concluído.
391. **periféricos, IRQ e testes** — documentar requisitos e referências técnicas.
392. **periféricos, IRQ e testes** — definir interfaces e invariantes do subsistema.
393. **periféricos, IRQ e testes** — implementar o caminho básico funcional.
394. **periféricos, IRQ e testes** — tratar limites, estados inválidos e erros.
395. **periféricos, IRQ e testes** — cobrir operações normais com testes unitários.
396. **periféricos, IRQ e testes** — adicionar casos extremos e regressões.
397. **periféricos, IRQ e testes** — integrar com o barramento e o scheduler.
398. **periféricos, IRQ e testes** — medir custo e corrigir gargalos sem alterar a semântica.
399. **periféricos, IRQ e testes** — registrar diagnóstico, limitações e evidências.
400. **periféricos, IRQ e testes** — executar validação de aceitação antes de marcar concluído.

## Bloco 5 — etapas 401–500: Integração, interface, biblioteca, saves e build Xenon
401. **integração dos dois SH-2** — documentar requisitos e referências técnicas.
402. **integração dos dois SH-2** — definir interfaces e invariantes do subsistema.
403. **integração dos dois SH-2** — implementar o caminho básico funcional.
404. **integração dos dois SH-2** — tratar limites, estados inválidos e erros.
405. **integração dos dois SH-2** — cobrir operações normais com testes unitários.
406. **integração dos dois SH-2** — adicionar casos extremos e regressões.
407. **integração dos dois SH-2** — integrar com o barramento e o scheduler.
408. **integração dos dois SH-2** — medir custo e corrigir gargalos sem alterar a semântica.
409. **integração dos dois SH-2** — registrar diagnóstico, limitações e evidências.
410. **integração dos dois SH-2** — executar validação de aceitação antes de marcar concluído.
411. **scheduler e sincronização global** — documentar requisitos e referências técnicas.
412. **scheduler e sincronização global** — definir interfaces e invariantes do subsistema.
413. **scheduler e sincronização global** — implementar o caminho básico funcional.
414. **scheduler e sincronização global** — tratar limites, estados inválidos e erros.
415. **scheduler e sincronização global** — cobrir operações normais com testes unitários.
416. **scheduler e sincronização global** — adicionar casos extremos e regressões.
417. **scheduler e sincronização global** — integrar com o barramento e o scheduler.
418. **scheduler e sincronização global** — medir custo e corrigir gargalos sem alterar a semântica.
419. **scheduler e sincronização global** — registrar diagnóstico, limitações e evidências.
420. **scheduler e sincronização global** — executar validação de aceitação antes de marcar concluído.
421. **integração VDP1/VDP2** — documentar requisitos e referências técnicas.
422. **integração VDP1/VDP2** — definir interfaces e invariantes do subsistema.
423. **integração VDP1/VDP2** — implementar o caminho básico funcional.
424. **integração VDP1/VDP2** — tratar limites, estados inválidos e erros.
425. **integração VDP1/VDP2** — cobrir operações normais com testes unitários.
426. **integração VDP1/VDP2** — adicionar casos extremos e regressões.
427. **integração VDP1/VDP2** — integrar com o barramento e o scheduler.
428. **integração VDP1/VDP2** — medir custo e corrigir gargalos sem alterar a semântica.
429. **integração VDP1/VDP2** — registrar diagnóstico, limitações e evidências.
430. **integração VDP1/VDP2** — executar validação de aceitação antes de marcar concluído.
431. **integração CD/SCSP/SMPC** — documentar requisitos e referências técnicas.
432. **integração CD/SCSP/SMPC** — definir interfaces e invariantes do subsistema.
433. **integração CD/SCSP/SMPC** — implementar o caminho básico funcional.
434. **integração CD/SCSP/SMPC** — tratar limites, estados inválidos e erros.
435. **integração CD/SCSP/SMPC** — cobrir operações normais com testes unitários.
436. **integração CD/SCSP/SMPC** — adicionar casos extremos e regressões.
437. **integração CD/SCSP/SMPC** — integrar com o barramento e o scheduler.
438. **integração CD/SCSP/SMPC** — medir custo e corrigir gargalos sem alterar a semântica.
439. **integração CD/SCSP/SMPC** — registrar diagnóstico, limitações e evidências.
440. **integração CD/SCSP/SMPC** — executar validação de aceitação antes de marcar concluído.
441. **interface principal** — documentar requisitos e referências técnicas.
442. **interface principal** — definir interfaces e invariantes do subsistema.
443. **interface principal** — implementar o caminho básico funcional.
444. **interface principal** — tratar limites, estados inválidos e erros.
445. **interface principal** — cobrir operações normais com testes unitários.
446. **interface principal** — adicionar casos extremos e regressões.
447. **interface principal** — integrar com o barramento e o scheduler.
448. **interface principal** — medir custo e corrigir gargalos sem alterar a semântica.
449. **interface principal** — registrar diagnóstico, limitações e evidências.
450. **interface principal** — executar validação de aceitação antes de marcar concluído.
451. **scanner de ROMs e metadados** — documentar requisitos e referências técnicas.
452. **scanner de ROMs e metadados** — definir interfaces e invariantes do subsistema.
453. **scanner de ROMs e metadados** — implementar o caminho básico funcional.
454. **scanner de ROMs e metadados** — tratar limites, estados inválidos e erros.
455. **scanner de ROMs e metadados** — cobrir operações normais com testes unitários.
456. **scanner de ROMs e metadados** — adicionar casos extremos e regressões.
457. **scanner de ROMs e metadados** — integrar com o barramento e o scheduler.
458. **scanner de ROMs e metadados** — medir custo e corrigir gargalos sem alterar a semântica.
459. **scanner de ROMs e metadados** — registrar diagnóstico, limitações e evidências.
460. **scanner de ROMs e metadados** — executar validação de aceitação antes de marcar concluído.
461. **capas, favoritos e filtros** — documentar requisitos e referências técnicas.
462. **capas, favoritos e filtros** — definir interfaces e invariantes do subsistema.
463. **capas, favoritos e filtros** — implementar o caminho básico funcional.
464. **capas, favoritos e filtros** — tratar limites, estados inválidos e erros.
465. **capas, favoritos e filtros** — cobrir operações normais com testes unitários.
466. **capas, favoritos e filtros** — adicionar casos extremos e regressões.
467. **capas, favoritos e filtros** — integrar com o barramento e o scheduler.
468. **capas, favoritos e filtros** — medir custo e corrigir gargalos sem alterar a semântica.
469. **capas, favoritos e filtros** — registrar diagnóstico, limitações e evidências.
470. **capas, favoritos e filtros** — executar validação de aceitação antes de marcar concluído.
471. **save state e save data** — documentar requisitos e referências técnicas.
472. **save state e save data** — definir interfaces e invariantes do subsistema.
473. **save state e save data** — implementar o caminho básico funcional.
474. **save state e save data** — tratar limites, estados inválidos e erros.
475. **save state e save data** — cobrir operações normais com testes unitários.
476. **save state e save data** — adicionar casos extremos e regressões.
477. **save state e save data** — integrar com o barramento e o scheduler.
478. **save state e save data** — medir custo e corrigir gargalos sem alterar a semântica.
479. **save state e save data** — registrar diagnóstico, limitações e evidências.
480. **save state e save data** — executar validação de aceitação antes de marcar concluído.
481. **toolchain ELF32 e default.xex** — documentar requisitos e referências técnicas.
482. **toolchain ELF32 e default.xex** — definir interfaces e invariantes do subsistema.
483. **toolchain ELF32 e default.xex** — implementar o caminho básico funcional.
484. **toolchain ELF32 e default.xex** — tratar limites, estados inválidos e erros.
485. **toolchain ELF32 e default.xex** — cobrir operações normais com testes unitários.
486. **toolchain ELF32 e default.xex** — adicionar casos extremos e regressões.
487. **toolchain ELF32 e default.xex** — integrar com o barramento e o scheduler.
488. **toolchain ELF32 e default.xex** — medir custo e corrigir gargalos sem alterar a semântica.
489. **toolchain ELF32 e default.xex** — registrar diagnóstico, limitações e evidências.
490. **toolchain ELF32 e default.xex** — executar validação de aceitação antes de marcar concluído.
491. **CI, artefatos e validação em hardware** — documentar requisitos e referências técnicas.
492. **CI, artefatos e validação em hardware** — definir interfaces e invariantes do subsistema.
493. **CI, artefatos e validação em hardware** — implementar o caminho básico funcional.
494. **CI, artefatos e validação em hardware** — tratar limites, estados inválidos e erros.
495. **CI, artefatos e validação em hardware** — cobrir operações normais com testes unitários.
496. **CI, artefatos e validação em hardware** — adicionar casos extremos e regressões.
497. **CI, artefatos e validação em hardware** — integrar com o barramento e o scheduler.
498. **CI, artefatos e validação em hardware** — medir custo e corrigir gargalos sem alterar a semântica.
499. **CI, artefatos e validação em hardware** — registrar diagnóstico, limitações e evidências.
500. **CI, artefatos e validação em hardware** — executar validação de aceitação antes de marcar concluído.


## Critérios globais de aceitação
1. Testes host com avisos tratados como erros e regressões anteriores preservadas.
2. Build Xenon ELF32 e pacote `default.xex` gerados no workflow.
3. Artefatos publicados com logs/checksums e ligação ao commit de origem.
4. Nenhuma alegação de boot físico ou jogo compatível sem evidência de teste em console.
5. Cada subsistema permanece marcado como parcial até implementar comportamento real específico do hardware Saturn.


## Registro de progresso — v0.46b

- **Implementado no repositório:** comparação imediata SH-2 `CMP/EQ #imm,R0` e seis formas de transferência byte/palavra/palavra longa relativas ao GBR.
- **Testes adicionados:** imediato assinado; leitura/escrita GBR relativa; extensão de sinal de byte/palavra e acessos de 8/16/32 bits.
- **Commits:** core SH-2 `9ff6db75f294587702f80af7a8273be7f6756be9`; testes `7e69c15a48a49842b93735e3d6d0071168e07a51`; notas de versão `72cfbaf8e5527e0d54fd74a2826335c04663718d`; README `f5e0de7e20182d97c72088dcc528ea838de225c2`.
- **Status:** código versionado; não declarar CI aprovado até consultar uma execução que inclua estes commits. Não há teste de boot em hardware real.
