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
    ],

    sidebar: [
      {
        text: 'User Guide',
        items: [
          { text: 'Installation', link: '/install' },
        ]
      },
      {
        text: 'Developer Guide',
        items: [
          { text: 'Getting Started', link: '/develop' },
        ]
      }
    ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/ezhai/nautilus-avinfo' }
    ]
  }
})
