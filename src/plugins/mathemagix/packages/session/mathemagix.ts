<TeXmacs|1.0.7>

<style|source>

<\body>
  <active*|<\src-title>
    <src-package|mathemagix|1.0>

    <\src-purpose>
      Markup for Mathemagix sessions.
    </src-purpose>

    <src-copyright|2002--2004|Joris van der Hoeven>

    <\src-license>
      This <TeXmacs> style package falls under the <hlink|GNU general public
      license|$TEXMACS_PATH/LICENSE> and comes WITHOUT ANY WARRANTY
      WHATSOEVER. If you do not have a copy of the license, then write to the
      Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
      02111-1307, USA.
    </src-license>
  </src-title>>

  <assign|mmx-prompt|<macro|nr|<with|color|red|<arg|nr>]<specific|html|&nbsp;>
  >>>

  <assign|mmx-prompt|<macro|nr|<with|mode|math|Mmx\<rangle\><specific|html|&nbsp;>
  \ >>>

  <assign|mmx-prompt|<macro|nr|<with|prog-language|verbatim|Mmx]<specific|html|&nbsp;>
  >>>

  <assign|mathemagix-input|<macro|prompt|body|<style-with|src-compact|none|<generic-input|<with|color|red|<arg|prompt>>|<with|color|dark
  brown|<arg|body>>>>>>

  <assign|mathemagix-output|<macro|body|<generic-output|<with|prog-language|verbatim|<arg|body>>>>>
</body>

<\initial>
  <\collection>
    <associate|preamble|true>
  </collection>
</initial>