import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "Nautilus AVInfo",
  description: "Documentation for the Nautilus AVInfo extension.",
  base: '/nautilus-avinfo/',
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Guide', link: '/guide/installation' },
      { text: 'Reference', link: '/reference/getting-started' },
    ],
    sidebar: [
      {
        text: "User Guide",
        items: [
          { text: 'Installation', link: '/guide/installation' },
        ]
      },
      {
        text: 'Developer Guide',
        items: [
          { text: 'Getting Started', link: '/reference/getting-started' },
        ]
      }
    ],
    socialLinks: [
      { icon: 'github', link: 'https://github.com/ezhai/nautilus-avinfo' }
    ]
  }
})
