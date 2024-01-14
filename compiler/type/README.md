### Function application:

$$
\begin{prooftree}
\AXC{$\Gamma \vdash e_1 : \tau_1 \rightarrow \tau_2 \quad \Gamma \vdash e_2 : \tau_1$}
\UnaryInfC{$\Gamma \vdash e_1 e_2 : \tau_2$}
\end{prooftree}
$$

### Pattern maching:

$$
\begin{prooftree}
\AXC{$\Gamma \vdash e : \tau \quad match_\tau(\tau, p_i) = b_i \quad \Gamma, b_i \vdash e_i : \tau_c $}
\UnaryInfC{$\Gamma \vdash match \enspace e \enspace with \enspace \{(p_1, e_1) ... (p_n, e_n)\} : \tau_c$}
\end{prooftree}
$$

$$
\begin{prooftree}
\AXC{}
\UnaryInfC{$match_\tau(\tau, v) = \{v : \tau\}$}
\end{prooftree}
$$

$$
\begin{prooftree}
\AXC{$\Gamma \vdash c : \tau_1 \rightarrow ... \rightarrow \tau_n \rightarrow \tau$}
\UnaryInfC{$match_\tau(\tau, c \enspace v_1...v_n) = \{v_1 : \tau_1 ...v_n : \tau_n\}$}
\end{prooftree}
$$
